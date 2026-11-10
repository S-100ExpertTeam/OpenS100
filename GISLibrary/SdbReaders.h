#pragma once

#include "../SystemDatabase/Inspection.h"

#include <filesystem>
#include <optional>
#include <vector>

namespace S100
{
	class DataCoverage;
}

// The System Database importer's readers for the formats this library parses
// (Documents/TechDocs/2026-11-10-system-database.md, SDB-3). Each reads only what the importer
// needs; none keeps the parsed data.
namespace GISLibrary
{
	// ISO 8211 datasets of S-100 products (S-101): the DSID record, the DataCoverage features and
	// the support files the features and information types refer to. Reads without a feature
	// catalogue: codes come from the dataset's own code tables.
	class S101DatasetInspector final : public s100::sdb::DatasetInspector
	{
	public:
		[[nodiscard]] bool accepts(const std::filesystem::path& file) const override;
		[[nodiscard]] std::optional<s100::sdb::InspectedDataset> inspect(const std::filesystem::path& file) const override;
	};

	// Feature catalogues and portrayal catalogues, from the top of their XML only.
	class XmlCatalogueInspector final : public s100::sdb::CatalogueInspector
	{
	public:
		[[nodiscard]] std::optional<s100::sdb::InspectedCatalogue> inspectFeatureCatalogue(const std::filesystem::path& xml) const override;
		[[nodiscard]] std::optional<s100::sdb::InspectedCatalogue> inspectPortrayalCatalogue(const std::filesystem::path& xml) const override;
	};

	// S-100 Part 17 exchange set catalogues (CATALOG.XML).
	class ExchangeCatalogueReader final : public s100::sdb::ExchangeSetReader
	{
	public:
		[[nodiscard]] std::optional<s100::sdb::InspectedExchangeSet> read(const std::filesystem::path& catalogXml) const override;
	};

	// The S100XC:dataCoverage of an exchange catalogue entry, one per coverage in the same order
	// (a coverage whose polygon cannot be read has no rings). EPSG:4326 positions are latitude
	// first; only an explicit CRS84 lists longitude first.
	[[nodiscard]] std::vector<s100::sdb::InspectedCoverage> exchangeCatalogueCoverages(const std::vector<S100::DataCoverage>& coverages);
}
