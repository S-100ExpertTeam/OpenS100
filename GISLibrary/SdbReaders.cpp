#include "stdafx.h"
#include "SdbReaders.h"

#include "ATTR.h"
#include "ExchangeCatalogue.h"
#include "F_RIAS.h"
#include "F_SPAS.h"
#include "H5_RootGroup.h"
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
#include <cmath>

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

		// --- GML -----------------------------------------------------------------------------

		// An attribute of the node or of its nearest ancestor that has it (srsName, srsDimension).
		std::string_view inheritedAttribute(pugi::xml_node node, const char* name)
		{
			for (; node; node = node.parent())
			{
				if (const pugi::xml_attribute attribute = node.attribute(name))
				{
					return attribute.value();
				}
			}
			return {};
		}

		// OGC CRS84 lists longitude first; EPSG:4326, which S-100 Part 10b prescribes and which a
		// position without srsName is taken to be, latitude first.
		bool latitudeFirst(std::string_view srsName)
		{
			return srsName.find("CRS84") == std::string_view::npos && srsName.find("crs84") == std::string_view::npos;
		}

		std::vector<double> parseNumbers(std::string_view text)
		{
			std::vector<double> numbers;
			std::size_t at = 0;
			while (at < text.size())
			{
				while (at < text.size() && std::isspace(static_cast<unsigned char>(text[at]))) ++at;
				if (at == text.size()) break;
				double value = 0;
				const auto [end, error] = std::from_chars(text.data() + at, text.data() + text.size(), value);
				if (error != std::errc())
				{
					return {};   // not a list of numbers
				}
				numbers.push_back(value);
				at = static_cast<std::size_t>(end - text.data());
			}
			return numbers;
		}

		void extend(std::optional<s100::sdb::BoundingBox>& bounds, double lon, double lat)
		{
			if (!bounds)
			{
				bounds = s100::sdb::BoundingBox{ lon, lat, lon, lat };
				return;
			}
			bounds->minX = (std::min)(bounds->minX, lon);
			bounds->minY = (std::min)(bounds->minY, lat);
			bounds->maxX = (std::max)(bounds->maxX, lon);
			bounds->maxY = (std::max)(bounds->maxY, lat);
		}

		// The tuples of one position list, in the axis order its srsName gives, or the other way
		// round when only that gives latitudes within +-90. Empty for anything else.
		std::vector<Position> readPositions(const std::vector<double>& numbers, std::size_t dimension, bool latFirst)
		{
			if (dimension < 2 || numbers.size() < dimension)
			{
				return {};
			}
			const std::size_t count = numbers.size() / dimension;
			const auto fits = [&](bool latitudeAtFirst) {
				for (std::size_t i = 0; i < count; ++i)
				{
					const double lat = numbers[i * dimension + (latitudeAtFirst ? 0 : 1)];
					const double lon = numbers[i * dimension + (latitudeAtFirst ? 1 : 0)];
					if (!(std::abs(lat) <= 90) || !(std::abs(lon) <= 180))
					{
						return false;
					}
				}
				return true;
			};
			if (!fits(latFirst))
			{
				if (!fits(!latFirst))
				{
					return {};   // not geographic coordinates
				}
				latFirst = !latFirst;
			}
			std::vector<Position> positions;
			positions.reserve(count);
			for (std::size_t i = 0; i < count; ++i)
			{
				positions.push_back(Position{ numbers[i * dimension + (latFirst ? 1 : 0)], numbers[i * dimension + (latFirst ? 0 : 1)] });
			}
			return positions;
		}

		// The radius of a gml:CircleByCenterPoint or gml:ArcByCenterPoint in metres; 0 when it
		// cannot be read.
		double radiusInMetres(const pugi::xml_node& segment)
		{
			const pugi::xml_node radius = childByLocalName(segment, "radius");
			const std::vector<double> value = parseNumbers(radius.child_value());
			if (value.size() != 1 || !(value[0] > 0))
			{
				return 0;
			}
			const std::string_view unit = radius.attribute("uom").value();
			if (unit == "km") return value[0] * 1000;
			if (unit == "[nmi_i]" || unit == "NM" || unit == "nmi" || unit == "M") return value[0] * 1852;
			return value[0];   // "m", and nothing said
		}

		// The bounding box of the geometry positions, and failing them of the envelope corners (an
		// envelope alone may be a placeholder; the geometry is what is drawn).
		std::optional<s100::sdb::BoundingBox> positionBounds(pugi::xml_node root)
		{
			std::optional<s100::sdb::BoundingBox> positions;
			std::optional<s100::sdb::BoundingBox> corners;
			struct Walker final : pugi::xml_tree_walker
			{
				std::optional<s100::sdb::BoundingBox>* positions = nullptr;
				std::optional<s100::sdb::BoundingBox>* corners = nullptr;

				bool for_each(pugi::xml_node& node) override
				{
					if (node.type() != pugi::node_element)
					{
						return true;
					}
					const std::string_view name = localName(node.name());
					const bool list = name == "posList";
					const bool corner = name == "lowerCorner" || name == "upperCorner";
					if (!list && !corner && name != "pos")
					{
						return true;
					}
					const std::vector<double> numbers = parseNumbers(node.child_value());
					std::size_t dimension = 2;
					if (const auto declared = parseInt(inheritedAttribute(node, "srsDimension")); declared && *declared >= 2)
					{
						dimension = static_cast<std::size_t>(*declared);
					}
					else if (!list && (numbers.size() == 2 || numbers.size() == 3))
					{
						dimension = numbers.size();   // a single position: x y, or x y depth
					}
					std::optional<s100::sdb::BoundingBox>& bounds = corner ? *corners : *positions;
					const auto read = readPositions(numbers, dimension, latitudeFirst(inheritedAttribute(node, "srsName")));
					for (const Position& position : read)
					{
						extend(bounds, position.lon, position.lat);
					}
					// A circle (S-124 warning areas) reaches its radius around the centre.
					const std::string_view segment = localName(node.parent().name());
					if (read.size() == 1 && (segment == "CircleByCenterPoint" || segment == "ArcByCenterPoint"))
					{
						constexpr double metresPerDegree = 111320.0;
						const double dLat = radiusInMetres(node.parent()) / metresPerDegree;
						const double dLon = dLat / (std::max)(std::cos(read[0].lat * 3.14159265358979323846 / 180), 0.01);
						extend(bounds, read[0].lon - dLon, read[0].lat - dLat);
						extend(bounds, read[0].lon + dLon, read[0].lat + dLat);
					}
					return true;
				}
			} walker;
			walker.positions = &positions;
			walker.corners = &corners;
			root.traverse(walker);
			return positions ? positions : corners;
		}

		// The product a GML dataset's root element names by its namespace,
		// "http://www.iho.int/S124/gml/2.0" -> "S-124".
		std::string productOfNamespace(const pugi::xml_node& root)
		{
			const std::string_view name(root.name());
			const auto colon = name.find(':');
			const std::string attribute = colon == std::string_view::npos ? std::string("xmlns") : "xmlns:" + std::string(name.substr(0, colon));
			return productFromIdentifier(root.attribute(attribute.c_str()).value());
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

		// Where a catalogue entry's file is: Edition 4.0 names the folder in filePath and only the
		// file in fileName; later editions put the whole path in fileName.
		std::string entryLocation(const std::string& filePath, const std::string& fileName)
		{
			if (filePath.empty())
			{
				return fileName;
			}
			std::string location = filePath;
			if (location.back() != '/' && location.back() != '\\')
			{
				location += '/';
			}
			return location + fileName;
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
			// Portrayal catalogues are delivered as archives (feature catalogues as XML).
			std::string extension = s100::sdb::toUtf8(file.extension());
			std::ranges::transform(extension, extension.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
			if (extension == ".zip")
			{
				return s100::sdb::CatalogueKind::Portrayal;
			}
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

	bool Hdf5DatasetInspector::accepts(const fs::path& file) const
	{
		std::string extension = s100::sdb::toUtf8(file.extension());
		std::ranges::transform(extension, extension.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
		return extension == ".h5" || extension == ".hdf5";
	}

	std::optional<InspectedDataset> Hdf5DatasetInspector::inspect(const fs::path& file) const
	{
		// A file that is not HDF5 is an expected answer here, not an error to print.
		H5E_auto2_t errorHandler = nullptr;
		void* errorData = nullptr;
		H5Eget_auto2(H5E_DEFAULT, &errorHandler, &errorData);
		H5Eset_auto2(H5E_DEFAULT, nullptr, nullptr);
		const hid_t fileId = H5Fopen(s100::sdb::toUtf8(file).c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
		H5_RootGroup root;
		if (fileId >= 0)
		{
			root.Read(fileId);
			H5Fclose(fileId);
		}
		H5Eset_auto2(H5E_DEFAULT, errorHandler, errorData);
		if (fileId < 0)
		{
			return std::nullopt;
		}

		// productSpecification is "INT.IHO.S-102.2.2": the product, then its edition.
		InspectedDataset dataset;
		dataset.name = s100::sdb::toUtf8(file.stem());
		dataset.product = productFromIdentifier(root.productSpecification);
		if (!dataset.product.empty())
		{
			const auto at = root.productSpecification.find(dataset.product.substr(2));
			const auto dot = at == std::string::npos ? std::string::npos : root.productSpecification.find('.', at);
			if (dot != std::string::npos)
			{
				dataset.productEdition = root.productSpecification.substr(dot + 1);
			}
		}
		else
		{
			// No usable productSpecification: S-100 file names start with the product number.
			const std::string& name = dataset.name;
			dataset.product = name.size() >= 3 ? productFromIdentifier(name.substr(0, 3)) : std::string();
		}
		if (dataset.product.empty())
		{
			return std::nullopt;   // HDF5, but not an S-100 dataset
		}
		dataset.format = s100::sdb::DatasetFormat::Hdf5;
		dataset.issueDate = isoDate(root.issueDate);

		const double west = root.westBoundLongitude;
		const double east = root.eastBoundLongitude;
		const double south = root.southBoundLatitude;
		const double north = root.northBoundLatitude;
		if (west < east && south < north)
		{
			dataset.bounds = s100::sdb::BoundingBox{ west, south, east, north };
			InspectedCoverage coverage;
			coverage.rings.push_back(Ring{ { west, south }, { east, south }, { east, north }, { west, north }, { west, south } });
			dataset.coverages.push_back(std::move(coverage));
		}
		return dataset;
	}

	// --- GmlDatasetInspector -------------------------------------------------------------------

	std::optional<s100::sdb::BoundingBox> gmlPositionBounds(const char* xml)
	{
		pugi::xml_document document;
		if (!xml || !document.load_string(xml))
		{
			return std::nullopt;
		}
		return positionBounds(document.document_element());
	}

	bool GmlDatasetInspector::accepts(const fs::path& file) const
	{
		std::string extension = s100::sdb::toUtf8(file.extension());
		std::ranges::transform(extension, extension.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
		return extension == ".gml";
	}

	std::optional<InspectedDataset> GmlDatasetInspector::inspect(const fs::path& file) const
	{
		pugi::xml_document document;
		if (!document.load_file(file.c_str()))
		{
			return std::nullopt;
		}
		const pugi::xml_node root = document.document_element();
		// Part 10b: the root is the product's Dataset element (DataSet in some older schemas).
		std::string rootName(localName(root.name()));
		std::ranges::transform(rootName, rootName.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
		if (rootName != "dataset")
		{
			return std::nullopt;
		}

		InspectedDataset dataset;
		dataset.name = s100::sdb::toUtf8(file.stem());
		dataset.format = s100::sdb::DatasetFormat::Gml;

		// S-100 Part 10b 10b-9.3: optional, and left out by some producers (the KOR S-124).
		const pugi::xml_node identification = childByLocalName(root, "DatasetIdentificationInformation");
		if (identification)
		{
			dataset.product = productFromIdentifier(childByLocalName(identification, "productIdentifier").child_value());
			dataset.productEdition = childByLocalName(identification, "productEdition").child_value();
			dataset.issueDate = isoDate(childByLocalName(identification, "datasetReferenceDate").child_value());
			dataset.edition = parseInt(childByLocalName(identification, "editionNumber").child_value());
			dataset.updateNumber = parseInt(childByLocalName(identification, "updateNumber").child_value()).value_or(0);
			dataset.isUpdate = std::string_view(childByLocalName(identification, "datasetPurpose").child_value()) == "update";
		}
		if (dataset.product.empty())
		{
			dataset.product = productOfNamespace(root);
		}
		if (dataset.product.empty() && dataset.name.size() >= 3)
		{
			dataset.product = productFromIdentifier(dataset.name.substr(0, 3));
		}
		if (dataset.product.empty() || dataset.product == "S-100")
		{
			return std::nullopt;   // a Dataset, but of no S-100 product
		}

		dataset.bounds = positionBounds(root);
		if (dataset.bounds)
		{
			const auto& [west, south, east, north] = *dataset.bounds;
			if (west < east && south < north)
			{
				InspectedCoverage coverage;
				coverage.rings.push_back(Ring{ { west, south }, { east, south }, { east, north }, { west, north }, { west, south } });
				dataset.coverages.push_back(std::move(coverage));
			}
		}
		return dataset;
	}

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
			InspectedExchangeSet::Dataset dataset{ .file = resolveCatalogueFile(folder, entryLocation(entry.FilePath, entry.FileName)) };
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
			const std::string location = entryLocation(entry.FilePath, entry.FileName);
			InspectedExchangeSet::SupportFile supportFile{ .file = resolveCatalogueFile(folder, location) };
			supportFile.product = productFolder(location);
			if (supportFile.product.empty()) supportFile.product = setProduct;
			if (entry.RevisionStatus) supportFile.purpose = S100::S100_SupportFileRevisionStatusToString(*entry.RevisionStatus);
			if (entry.EditionNumber) supportFile.edition = *entry.EditionNumber;
			if (entry.DataType) supportFile.format = S100::SupportFileFormatToString(*entry.DataType);
			if (entry.IssueDate) supportFile.issueDate = entry.IssueDate->ToString();
			set.supportFiles.push_back(std::move(supportFile));
		}

		for (const auto& entry : catalogue.CatalogueDiscoveryMetadata)
		{
			const fs::path file = resolveCatalogueFile(folder, entryLocation(entry.FilePath, entry.FileName));
			if (const auto kind = catalogueKind(file))
			{
				set.catalogues.push_back({ .file = file, .kind = *kind });
			}
		}
		return set;
	}
}
