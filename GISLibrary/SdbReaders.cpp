#include "stdafx.h"
#include "SdbReaders.h"

#include "ATTR.h"
#include "ExchangeCatalogue.h"
#include "F_RIAS.h"
#include "F_SPAS.h"
#include "R_CompositeRecord.h"
#include "R_CurveRecord.h"
#include "R_FeatureRecord.h"
#include "R_InformationRecord.h"
#include "R_SurfaceRecord.h"
#include "RIAS.h"
#include "S101Cell.h"
#include "SPAS.h"

#include "../GeoMetryLibrary/TextCodec.h"
#include "../SystemDatabase/CatalogueResolution.h"
#include "../SystemDatabase/Utf8Path.h"

#include <pugixml.hpp>

#include <algorithm>
#include <cctype>
#include <charconv>

namespace GISLibrary
{
	namespace fs = std::filesystem;
	using s100::sdb::InspectedCatalogue;
	using s100::sdb::InspectedCoverage;
	using s100::sdb::InspectedDataset;
	using s100::sdb::InspectedExchangeSet;
	using s100::sdb::Position;
	using s100::sdb::Ring;
	using s100::sdb::productFromIdentifier;

	namespace
	{
		bool isDigit(char c)
		{
			return std::isdigit(static_cast<unsigned char>(c)) != 0;
		}

		std::optional<int> parseInt(std::string_view text)
		{
			while (!text.empty() && text.front() == ' ') text.remove_prefix(1);
			while (!text.empty() && text.back() == ' ') text.remove_suffix(1);
			int value = 0;
			const auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), value);
			if (error != std::errc() || end != text.data() + text.size())
			{
				return std::nullopt;
			}
			return value;
		}

		// "20260703" -> "2026-07-03"; anything else is kept as it is.
		std::string isoDate(const std::string& text)
		{
			if (text.size() == 8 && std::ranges::all_of(text, isDigit))
			{
				return text.substr(0, 4) + "-" + text.substr(4, 2) + "-" + text.substr(6, 2);
			}
			return text;
		}

		// The local part of an XML name: "S100FC:productId" -> "productId".
		std::string_view localName(const char* name)
		{
			std::string_view text(name);
			const auto colon = text.find(':');
			return colon == std::string_view::npos ? text : text.substr(colon + 1);
		}

		pugi::xml_node childByLocalName(const pugi::xml_node& parent, std::string_view name)
		{
			for (pugi::xml_node child : parent.children())
			{
				if (localName(child.name()) == name)
				{
					return child;
				}
			}
			return {};
		}

		// --- ISO 8211 ------------------------------------------------------------------------

		constexpr int curveRecord = 120;
		constexpr int compositeCurveRecord = 125;
		constexpr int surfaceRecord = 130;

		// The rings of a surface, one per RIAS entry (exterior and interiors), in degrees.
		std::vector<Ring> surfaceRings(S101Cell& cell, const R_FeatureRecord& feature)
		{
			const double cmfx = cell.GetCMFX() > 0 ? cell.GetCMFX() : 10000000.0;
			const double cmfy = cell.GetCMFY() > 0 ? cell.GetCMFY() : 10000000.0;
			std::vector<Ring> rings;
			for (const F_SPAS* spasField : feature.m_spas)
			{
				for (SPAS* spas : spasField->m_arr)
				{
					if (spas->m_name.RCNM != surfaceRecord)
					{
						continue;
					}
					R_SurfaceRecord* surface = cell.GetSurfaceRecord(spas->m_name.GetName());
					if (!surface)
					{
						continue;
					}
					for (const F_RIAS* riasField : surface->m_rias)
					{
						for (RIAS* rias : riasField->m_arr)
						{
							std::vector<POINT> points;
							if (rias->m_name.RCNM == curveRecord)
							{
								if (auto* curve = cell.GetCurveRecord(rias->m_name.GetName()))
								{
									cell.GetFullSpatialData(curve, points, rias->m_ornt);
								}
							}
							else if (rias->m_name.RCNM == compositeCurveRecord)
							{
								if (auto* composite = cell.GetCompositeCurveRecord(rias->m_name.GetName()))
								{
									cell.GetFullSpatialData(composite, points, rias->m_ornt);
								}
							}
							if (points.size() < 3)
							{
								continue;
							}
							Ring ring;
							ring.reserve(points.size());
							for (const POINT& point : points)
							{
								ring.push_back(Position{ point.x / cmfx, point.y / cmfy });
							}
							rings.push_back(std::move(ring));
						}
					}
				}
			}
			return rings;
		}

		bool isFileReference(const std::string& code)
		{
			return code == "fileReference" || code == "pictorialRepresentation";
		}

		void collectFileReferences(S101Cell& cell, std::vector<ATTR*> attributes, std::vector<std::string>& references)
		{
			for (ATTR* attribute : attributes)
			{
				if (!attribute || !isFileReference(cell.m_dsgir.GetAttributeCodeAsString(attribute->m_natc)))
				{
					continue;
				}
				std::string value = attribute->getValueAsString();
				if (!value.empty() && std::ranges::find(references, value) == references.end())
				{
					references.push_back(std::move(value));
				}
			}
		}

		// --- exchange catalogue --------------------------------------------------------------

		// "file:/S-101/DATASET_FILES/x/x.000" -> <folder>/S-101/DATASET_FILES/x/x.000
		fs::path resolveCatalogueFile(const fs::path& folder, const std::string& uri)
		{
			std::string_view text(uri);
			for (const std::string_view prefix : { "file:///", "file:/" })
			{
				if (text.starts_with(prefix))
				{
					text.remove_prefix(prefix.size());
					break;
				}
			}
			while (!text.empty() && (text.front() == '/' || text.front() == '\\'))
			{
				text.remove_prefix(1);
			}
			std::string relative(text);
			std::ranges::replace(relative, '\\', '/');
			return (folder / s100::sdb::fromUtf8(relative)).lexically_normal();
		}

		// The product folder a file was delivered in ("S-101" of "S-101/SUPPORT_FILES/x.TXT").
		std::string productFolder(const std::string& uri)
		{
			std::string_view text(uri);
			while (!text.empty())
			{
				const auto slash = text.find('/');
				const std::string_view segment = text.substr(0, slash);
				const std::string product = productFromIdentifier(segment);
				if (!product.empty() && segment.size() <= 6)
				{
					return product;
				}
				if (slash == std::string_view::npos)
				{
					break;
				}
				text.remove_prefix(slash + 1);
			}
			return {};
		}

		std::optional<s100::sdb::CatalogueKind> catalogueKind(const fs::path& file)
		{
			pugi::xml_document document;
			if (!document.load_file(file.c_str()))
			{
				return std::nullopt;
			}
			const std::string_view root = localName(document.document_element().name());
			if (root.find("FeatureCatalogue") != std::string_view::npos)
			{
				return s100::sdb::CatalogueKind::Feature;
			}
			if (root == "portrayalCatalog")
			{
				return s100::sdb::CatalogueKind::Portrayal;
			}
			return std::nullopt;
		}
	}

	// --- exchange catalogue coverage ----------------------------------------------------------

	std::vector<InspectedCoverage> exchangeCatalogueCoverages(const std::vector<S100::DataCoverage>& coverages)
	{
		std::vector<InspectedCoverage> result;
		for (const S100::DataCoverage& coverage : coverages)
		{
			InspectedCoverage item;
			if (coverage.OptimumDisplayScale) item.optimumDisplayScale = *coverage.OptimumDisplayScale;
			if (coverage.MinimumDisplayScale) item.minimumDisplayScale = *coverage.MinimumDisplayScale;
			if (coverage.MaximumDisplayScale) item.maximumDisplayScale = *coverage.MaximumDisplayScale;

			const auto& polygon = coverage.BoundingPolygon.Polygon;
			// EPSG:4326 lists latitude first; only an explicit CRS84 lists longitude first.
			const bool longitudeFirst = polygon.srsName.find("CRS84") != std::string::npos;
			for (const std::string& posList : polygon.Geom)
			{
				std::vector<double> values;
				std::string_view text(posList);
				while (!text.empty())
				{
					const auto start = text.find_first_not_of(" \t\r\n");
					if (start == std::string_view::npos)
					{
						break;
					}
					text.remove_prefix(start);
					const auto end = text.find_first_of(" \t\r\n");
					const std::string_view number = text.substr(0, end);
					double value = 0;
					const auto [last, error] = std::from_chars(number.data(), number.data() + number.size(), value);
					if (error != std::errc() || last != number.data() + number.size())
					{
						values.clear();
						break;
					}
					values.push_back(value);
					text.remove_prefix(number.size());
				}
				if (values.size() < 6 || values.size() % 2 != 0)
				{
					continue;
				}
				Ring ring;
				for (std::size_t i = 0; i < values.size(); i += 2)
				{
					ring.push_back(longitudeFirst ? Position{ values[i], values[i + 1] } : Position{ values[i + 1], values[i] });
				}
				item.rings.push_back(std::move(ring));
			}
			result.push_back(std::move(item));
		}
		return result;
	}

	// --- S101DatasetInspector ------------------------------------------------------------------

	bool S101DatasetInspector::accepts(const fs::path& file) const
	{
		const std::string extension = s100::sdb::toUtf8(file.extension());
		return extension.size() == 4 && extension[0] == '.' && isDigit(extension[1]) && isDigit(extension[2]) && isDigit(extension[3]);
	}

	std::optional<InspectedDataset> S101DatasetInspector::inspect(const fs::path& file) const
	{
		S101Cell cell;
		if (!cell.Read8211(file.wstring()))
		{
			return std::nullopt;
		}
		const auto& dsid = cell.m_dsgir.m_dsid;
		const std::string product = productFromIdentifier(s100::text::ToUtf8(dsid.m_prsp));
		if (product.empty())
		{
			return std::nullopt;   // not an S-100 ISO 8211 dataset
		}

		InspectedDataset dataset;
		dataset.name = s100::sdb::toUtf8(file.stem());
		dataset.product = product;
		dataset.productEdition = s100::text::ToUtf8(dsid.m_pred);
		dataset.format = s100::sdb::DatasetFormat::Iso8211;
		dataset.isUpdate = dsid.m_prof == L"2";
		dataset.issueDate = isoDate(s100::text::ToUtf8(dsid.m_dsrd));

		// DSED is "<edition>.<update>": "2.4" is edition 2 including update 4.
		const std::string edition = s100::text::ToUtf8(dsid.m_dsed);
		const auto dot = edition.find('.');
		dataset.edition = parseInt(std::string_view(edition).substr(0, dot));
		if (dot != std::string::npos)
		{
			dataset.updateNumber = parseInt(std::string_view(edition).substr(dot + 1)).value_or(0);
		}

		for (R_FeatureRecord* feature : cell.GetVecFeature())
		{
			collectFileReferences(cell, feature->GetAllAttributes(), dataset.supportFileReferences);
			if (cell.m_dsgir.GetFeatureCodeAsString(feature->GetNumericCode()) != "DataCoverage")
			{
				continue;
			}
			InspectedCoverage coverage;
			for (ATTR* attribute : feature->GetAllAttributes())
			{
				if (!attribute || attribute->m_paix != 0)
				{
					continue;
				}
				const std::string code = cell.m_dsgir.GetAttributeCodeAsString(attribute->m_natc);
				if (code == "optimumDisplayScale") coverage.optimumDisplayScale = parseInt(attribute->getValueAsString());
				else if (code == "minimumDisplayScale") coverage.minimumDisplayScale = parseInt(attribute->getValueAsString());
				else if (code == "maximumDisplayScale") coverage.maximumDisplayScale = parseInt(attribute->getValueAsString());
			}
			coverage.rings = surfaceRings(cell, *feature);
			if (!coverage.rings.empty())
			{
				dataset.coverages.push_back(std::move(coverage));
			}
		}
		for (R_InformationRecord* information : cell.GetVecInformation())
		{
			collectFileReferences(cell, information->GetAllAttributes(), dataset.supportFileReferences);
		}
		return dataset;
	}

	// --- XmlCatalogueInspector -----------------------------------------------------------------

	std::optional<InspectedCatalogue> XmlCatalogueInspector::inspectFeatureCatalogue(const fs::path& xml) const
	{
		pugi::xml_document document;
		if (!document.load_file(xml.c_str()))
		{
			return std::nullopt;
		}
		const pugi::xml_node root = document.document_element();
		if (localName(root.name()).find("FeatureCatalogue") == std::string_view::npos)
		{
			return std::nullopt;
		}
		InspectedCatalogue catalogue{
			.product = productFromIdentifier(childByLocalName(root, "productId").child_value()),
			.version = childByLocalName(root, "versionNumber").child_value(),
			.issueDate = childByLocalName(root, "versionDate").child_value(),
		};
		if (catalogue.product.empty() || catalogue.version.empty())
		{
			return std::nullopt;
		}
		return catalogue;
	}

	std::optional<InspectedCatalogue> XmlCatalogueInspector::inspectPortrayalCatalogue(const fs::path& xml) const
	{
		pugi::xml_document document;
		if (!document.load_file(xml.c_str()))
		{
			return std::nullopt;
		}
		const pugi::xml_node root = document.document_element();
		if (localName(root.name()) != "portrayalCatalog")
		{
			return std::nullopt;
		}
		InspectedCatalogue catalogue{
			.product = productFromIdentifier(root.attribute("productId").value()),
			.version = root.attribute("version").value(),
		};
		if (catalogue.product.empty() || catalogue.version.empty())
		{
			return std::nullopt;
		}
		return catalogue;
	}

	// --- ExchangeCatalogueReader ---------------------------------------------------------------

	std::optional<InspectedExchangeSet> ExchangeCatalogueReader::read(const fs::path& catalogXml) const
	{
		S100::ExchangeCatalogue catalogue;
		try
		{
			if (!catalogue.OpenFile(catalogXml))
			{
				return std::nullopt;
			}
		}
		catch (const std::exception&)
		{
			// The metadata parsers throw on values they do not know; report the catalogue
			// as unreadable rather than letting the exception escape the import.
			return std::nullopt;
		}
		const fs::path folder = catalogXml.parent_path();
		const std::string setProduct = catalogue.productSpecification
			? productFromIdentifier(catalogue.productSpecification->ProductIdentifier) : std::string();

		InspectedExchangeSet set;
		if (catalogue.Identifier)
		{
			set.identifier = catalogue.Identifier->Identifier;
			set.issueDate = catalogue.Identifier->DateTime.ToString();
		}

		for (const auto& entry : catalogue.DatasetDiscoveryMetadata)
		{
			InspectedExchangeSet::Dataset dataset{ .file = resolveCatalogueFile(folder, entry.FileName) };
			if (entry.productSpecification)
			{
				dataset.product = productFromIdentifier(entry.productSpecification->ProductIdentifier);
				if (entry.productSpecification->Version) dataset.productEdition = *entry.productSpecification->Version;
			}
			if (dataset.product.empty()) dataset.product = setProduct;
			if (entry.EditionNumber) dataset.edition = *entry.EditionNumber;
			if (entry.UpdateNumber) dataset.updateNumber = *entry.UpdateNumber;
			if (entry.IssueDate) dataset.issueDate = entry.IssueDate->ToString();
			if (entry.BoundingBox)
			{
				dataset.bounds = s100::sdb::BoundingBox{ entry.BoundingBox->WestBoundLongitude, entry.BoundingBox->SouthBoundLatitude,
					entry.BoundingBox->EastBoundLongitude, entry.BoundingBox->NorthBoundLatitude };
			}
			dataset.coverages = exchangeCatalogueCoverages(entry.dataCoverage);
			std::erase_if(dataset.coverages, [](const InspectedCoverage& coverage) { return coverage.rings.empty(); });
			set.datasets.push_back(std::move(dataset));
		}

		for (const auto& entry : catalogue.SupportFileDiscoveryMetadata)
		{
			InspectedExchangeSet::SupportFile supportFile{ .file = resolveCatalogueFile(folder, entry.FileName) };
			supportFile.product = productFolder(entry.FileName);
			if (supportFile.product.empty()) supportFile.product = setProduct;
			if (entry.RevisionStatus) supportFile.purpose = S100::S100_SupportFileRevisionStatusToString(*entry.RevisionStatus);
			if (entry.EditionNumber) supportFile.edition = *entry.EditionNumber;
			if (entry.DataType) supportFile.format = S100::SupportFileFormatToString(*entry.DataType);
			if (entry.IssueDate) supportFile.issueDate = entry.IssueDate->ToString();
			set.supportFiles.push_back(std::move(supportFile));
		}

		for (const auto& entry : catalogue.CatalogueDiscoveryMetadata)
		{
			const fs::path file = resolveCatalogueFile(folder, entry.FileName);
			if (const auto kind = catalogueKind(file))
			{
				set.catalogues.push_back({ .file = file, .kind = *kind });
			}
		}
		return set;
	}
}
