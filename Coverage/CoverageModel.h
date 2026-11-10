#pragma once

#include "CoverageTime.h"
#include "Geo.h"
#include "RegularGrid.h"

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

// Format-neutral model of an S-100 Part 10c gridded coverage dataset (S-102, S-104, S-111, ...):
// features hold a regular grid of values per time group and, for three-dimensional products, per
// depth level. Values are read on demand through CoverageSource (opens100-web
// `core/src/coverage.ts`).
namespace s100::coverage
{
    // One value channel of a feature: the whole value, or one member of a compound value.
    struct CoverageComponent
    {
        std::string name;    // "depth"
        std::string label;
        std::string uom;
        double fillValue = 0;
        std::optional<double> lower;
        std::optional<double> upper;
        // The range of the values over every record, as the dataset states it (S-104
        // minDatasetHeight/maxDatasetHeight); absent where the file does not say.
        std::optional<ValueRange> datasetRange;
    };

    struct CoverageFeature
    {
        std::string id;      // the instance path, unique within the dataset: "BathymetryCoverage/BathymetryCoverage.01"
        std::string code;    // "BathymetryCoverage"
        std::string label;
        std::vector<CoverageComponent> components;   // at least one; one per member of a compound value
        bool compound = false;
        std::optional<std::vector<double>> depthLevels;   // three-dimensional features only
        RegularGrid grid;
        std::vector<CoverageTimeGroup> timeGroups;

        // The index of the component named `name` (case as given), if any.
        [[nodiscard]] std::optional<std::size_t> componentIndex(std::string_view name) const noexcept
        {
            for (std::size_t i = 0; i < components.size(); ++i)
            {
                if (components[i].name == name) return i;
            }
            return std::nullopt;
        }
    };

    using MetadataValue = std::variant<std::string, double>;

    struct CoverageDataset
    {
        std::string name;
        std::string productId;              // "S-102"
        std::string productSpecification;   // "INT.IHO.S-102.3.0.0"
        std::optional<std::string> title;
        GeoBounds bounds;
        std::map<std::string, MetadataValue, std::less<>> metadata;   // every root attribute
        std::vector<CoverageFeature> features;

        [[nodiscard]] const CoverageFeature* feature(std::string_view id) const noexcept
        {
            for (const auto& item : features)
            {
                if (item.id == id) return &item;
            }
            return nullptr;
        }
        [[nodiscard]] std::optional<double> numberAttribute(std::string_view key) const;
        [[nodiscard]] std::optional<std::string> textAttribute(std::string_view key) const;
    };

    struct CoverageSelection
    {
        std::size_t timeIndex = 0;
        std::size_t depthIndex = 0;
        std::size_t component = 0;
    };

    using Values = std::shared_ptr<const std::vector<float>>;

    // A coverage dataset whose values are read on demand, one grid at a time.
    class CoverageSource
    {
    public:
        virtual ~CoverageSource() = default;

        [[nodiscard]] virtual const CoverageDataset& dataset() const noexcept = 0;
        // rows * cols values, row-major, row 0 at the grid origin; null when the feature or the
        // selection does not exist or cannot be read.
        [[nodiscard]] virtual Values readValues(std::string_view featureId, const CoverageSelection& selection) const = 0;
        // Increases whenever readValues starts returning different values, so drawings built
        // from them can be rebuilt.
        [[nodiscard]] virtual std::uint64_t revision() const noexcept { return 0; }
    };

    // "INT.IHO.S-413.1.0" -> "S-413"; std::nullopt when there is no product number.
    [[nodiscard]] std::optional<std::string> productIdFromSpecification(std::string_view specification);
}
