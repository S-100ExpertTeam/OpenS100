#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

// Rows of the System Database (Documents/TechDocs/2026-11-10-system-database.md, section 3.2).
//
// Strings are UTF-8. Every `path` is relative to the System Database root and uses '/' as the
// separator. Dates are ISO 8601 text as they appear in the source metadata; `importedAt` is the
// ISO 8601 UTC time the record was written. An `id` of 0 means "not stored yet".
namespace s100::sdb
{
    using RecordId = std::int64_t;

    struct BoundingBox
    {
        double minX = 0;
        double minY = 0;
        double maxX = 0;
        double maxY = 0;

        friend bool operator==(const BoundingBox&, const BoundingBox&) = default;
    };

    struct ExchangeSetRecord
    {
        RecordId id = 0;
        std::string sourcePath;    // where it was imported from; informational, may be absolute
        std::string identifier;
        std::string issueDate;
        std::string importedAt;

        friend bool operator==(const ExchangeSetRecord&, const ExchangeSetRecord&) = default;
    };

    // A feature catalogue or a portrayal catalogue; the two tables have the same shape.
    struct CatalogueRecord
    {
        RecordId id = 0;
        std::string product;       // e.g. "S-101"
        std::string version;       // e.g. "2.0.0"
        std::string path;          // FC: the catalogue XML; PC: portrayal_catalogue.xml in its folder
        std::string sha256;
        std::string issueDate;
        std::string importedAt;

        friend bool operator==(const CatalogueRecord&, const CatalogueRecord&) = default;
    };

    enum class DatasetFormat
    {
        Iso8211,
        Gml,
        Hdf5,
    };

    enum class DatasetStatus
    {
        Active,
        Cancelled,
    };

    [[nodiscard]] std::string_view toString(DatasetFormat format) noexcept;
    [[nodiscard]] std::optional<DatasetFormat> parseDatasetFormat(std::string_view text) noexcept;
    [[nodiscard]] std::string_view toString(DatasetStatus status) noexcept;
    [[nodiscard]] std::optional<DatasetStatus> parseDatasetStatus(std::string_view text) noexcept;

    struct DatasetRecord
    {
        RecordId id = 0;
        std::string name;          // file name without extension, e.g. "101KR003G2M00"
        std::string product;
        std::string productEdition;
        DatasetFormat format = DatasetFormat::Iso8211;
        std::optional<int> edition;
        int updateNumber = 0;
        std::string issueDate;
        std::string path;
        std::optional<BoundingBox> bounds;
        DatasetStatus status = DatasetStatus::Active;
        std::optional<RecordId> exchangeSetId;
        std::string importedAt;

        friend bool operator==(const DatasetRecord&, const DatasetRecord&) = default;
    };

    struct DatasetUpdateRecord
    {
        RecordId datasetId = 0;
        int updateNumber = 0;
        std::string path;
        std::string issueDate;
        bool applied = false;

        friend bool operator==(const DatasetUpdateRecord&, const DatasetUpdateRecord&) = default;
    };

    // One DataCoverage feature of a dataset. Scales are denominators, as encoded in the data.
    struct DataCoverageRecord
    {
        RecordId id = 0;
        RecordId datasetId = 0;
        std::optional<int> optimumDisplayScale;
        std::optional<int> minimumDisplayScale;
        std::optional<int> maximumDisplayScale;
        BoundingBox bounds;
        std::vector<std::uint8_t> polygon;   // WKB, longitude/latitude

        friend bool operator==(const DataCoverageRecord&, const DataCoverageRecord&) = default;
    };

    struct SupportFileRecord
    {
        RecordId id = 0;
        std::string name;
        std::string path;
        std::string purpose;       // S-100 supportFilePurpose: new, replacement, deletion
        std::optional<int> edition;
        std::string format;
        std::string issueDate;
        std::string sha256;
        std::string importedAt;

        friend bool operator==(const SupportFileRecord&, const SupportFileRecord&) = default;
    };
}
