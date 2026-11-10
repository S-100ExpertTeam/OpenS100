#pragma once

#include "CatalogueRepository.h"
#include "Geometry.h"
#include "Records.h"

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

// What the importer needs to know about a file before it installs it, and the interfaces of the
// readers that find it out. The System Database does not parse S-100 files itself: a host
// registers readers for the formats it supports (the ISO 8211 reader lives with the parser in
// GISLibrary), so adding a format means adding a reader, not changing the importer.
namespace s100::sdb
{
    // A DataCoverage feature (S-98 Appendix E-1): where the dataset is meant to be shown, and at
    // which scales. Scales are denominators, as encoded.
    struct InspectedCoverage
    {
        std::optional<int> optimumDisplayScale;
        std::optional<int> minimumDisplayScale;
        std::optional<int> maximumDisplayScale;
        PolygonRings rings;   // longitude/latitude, exterior first
    };

    struct InspectedDataset
    {
        std::string name;              // the file name without extension, "101KR003G2M00"
        std::string product;           // "S-101"
        std::string productEdition;    // edition of the product specification, "2.0.0"
        DatasetFormat format = DatasetFormat::Iso8211;
        std::optional<int> edition;    // 0 in a cancellation
        // An update file (.001, ...) rather than a base dataset. A base dataset may still carry an
        // update number above 0: a re-issue includes the updates up to it.
        bool isUpdate = false;
        int updateNumber = 0;
        std::string issueDate;
        std::optional<BoundingBox> bounds;
        std::vector<InspectedCoverage> coverages;
        // File names of the support files the data refers to (pictures, text, ...).
        std::vector<std::string> supportFileReferences;
    };

    class DatasetInspector
    {
    public:
        virtual ~DatasetInspector() = default;

        // Whether this reader handles files like this one. Decided from the name; cheap.
        [[nodiscard]] virtual bool accepts(const std::filesystem::path& file) const = 0;
        // The dataset's metadata and DataCoverage features; std::nullopt when the file cannot be
        // read as a dataset.
        [[nodiscard]] virtual std::optional<InspectedDataset> inspect(const std::filesystem::path& file) const = 0;
    };

    struct InspectedCatalogue
    {
        std::string product;   // "S-101"
        std::string version;   // "2.0.0"
        std::string issueDate;
    };

    class CatalogueInspector
    {
    public:
        virtual ~CatalogueInspector() = default;

        [[nodiscard]] virtual std::optional<InspectedCatalogue> inspectFeatureCatalogue(const std::filesystem::path& xml) const = 0;
        // `xml` is the portrayal_catalogue.xml at the top of the catalogue folder.
        [[nodiscard]] virtual std::optional<InspectedCatalogue> inspectPortrayalCatalogue(const std::filesystem::path& xml) const = 0;
    };

    // The contents of an exchange set catalogue (CATALOG.XML). File locations are absolute.
    struct InspectedExchangeSet
    {
        // What the catalogue says about a dataset. The importer reads the dataset itself and uses
        // these only for what the dataset does not say (coverage, edition, ...).
        struct Dataset
        {
            std::filesystem::path file;
            std::string product;
            std::string productEdition;
            std::optional<int> edition;
            std::optional<int> updateNumber;
            std::string issueDate;
            std::optional<BoundingBox> bounds;
            std::vector<InspectedCoverage> coverages;
        };

        struct SupportFile
        {
            std::filesystem::path file;
            std::string product;           // the product folder it was delivered in, "S-101"
            std::string purpose;           // new, replacement, deletion
            std::optional<int> edition;
            std::string format;
            std::string issueDate;
        };

        struct Catalogue
        {
            std::filesystem::path file;    // FC: the XML; PC: portrayal_catalogue.xml
            CatalogueKind kind = CatalogueKind::Feature;
        };

        std::string identifier;
        std::string issueDate;
        std::vector<Dataset> datasets;
        std::vector<SupportFile> supportFiles;
        std::vector<Catalogue> catalogues;
    };

    class ExchangeSetReader
    {
    public:
        virtual ~ExchangeSetReader() = default;

        [[nodiscard]] virtual std::optional<InspectedExchangeSet> read(const std::filesystem::path& catalogXml) const = 0;
    };
}
