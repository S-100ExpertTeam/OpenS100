#include "DatasetSelection.h"

#include "SystemDatabase.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <unordered_map>

namespace s100::sdb
{
    namespace
    {
        // S-98 Table 11 bands 2-15 as {minimum scale, optimum scale} denominators. Band 1 is
        // everything smaller than 1:10,000,000.
        constexpr std::array<std::array<double, 2>, 14> bandLimits = { {
            { 10'000'000, 3'500'000 },
            { 3'500'000, 1'500'000 },
            { 1'500'000, 700'000 },
            { 700'000, 350'000 },
            { 350'000, 180'000 },
            { 180'000, 90'000 },
            { 90'000, 45'000 },
            { 45'000, 22'000 },
            { 22'000, 12'000 },
            { 12'000, 8'000 },
            { 8'000, 4'000 },
            { 4'000, 3'000 },
            { 3'000, 2'000 },
            { 2'000, 1'000 },
        } };

        constexpr double smallestBandedScale = 10'000'000;

        bool finite(const DevicePoint& point) noexcept
        {
            return std::isfinite(point.x) && std::isfinite(point.y);
        }

        std::vector<std::vector<DevicePoint>> project(const SelectableCoverage& coverage, const DeviceProjection& projection)
        {
            std::vector<std::vector<DevicePoint>> rings;
            rings.reserve(coverage.rings.size());
            for (const Ring& ring : coverage.rings)
            {
                std::vector<DevicePoint> points;
                points.reserve(ring.size());
                for (const Position& position : ring)
                {
                    points.push_back(projection(position));
                }
                // A ring that does not project (beyond the projection's limits) cannot be filled.
                if (std::ranges::all_of(points, finite))
                {
                    rings.push_back(std::move(points));
                }
            }
            return rings;
        }
    }

    int scaleBandOf(double scale) noexcept
    {
        if (scale >= smallestBandedScale)
        {
            return 1;
        }
        for (int index = 2; index <= 15; ++index)
        {
            const auto& [minimumScale, optimumScale] = bandLimits[static_cast<std::size_t>(index - 2)];
            if (scale < minimumScale && scale >= optimumScale)
            {
                return index;
            }
        }
        return 15;
    }

    ScaleBands scaleBandsOf(std::optional<int> minimumDisplayScale, int optimumDisplayScale) noexcept
    {
        const double minimum = minimumDisplayScale ? *minimumDisplayScale : std::numeric_limits<double>::infinity();
        const double optimum = optimumDisplayScale;
        ScaleBands bands;
        if (minimum > smallestBandedScale)
        {
            bands.set(1);
        }
        for (int index = 2; index <= 15; ++index)
        {
            const auto& [minimumScale, optimumScale] = bandLimits[static_cast<std::size_t>(index - 2)];
            // max() of two scales is the smaller denominator, min() the larger.
            if (std::min(minimum, minimumScale) > std::max(optimum, optimumScale))
            {
                bands.set(static_cast<std::size_t>(index));
            }
        }
        return bands;
    }

    // --- DeviceRegion ----------------------------------------------------------------------------

    DeviceRegion::DeviceRegion(double width, double height, double cellSize)
        : cellSize_(cellSize),
          columns_(std::max(0, static_cast<int>(std::ceil(width / cellSize)))),
          rows_(std::max(0, static_cast<int>(std::ceil(height / cellSize)))),
          cells_(static_cast<std::size_t>(columns_) * static_cast<std::size_t>(rows_), std::uint8_t{ 1 }),
          remaining_(cells_.size())
    {
    }

    bool DeviceRegion::subtract(std::span<const std::vector<DevicePoint>> rings)
    {
        double minY = std::numeric_limits<double>::infinity();
        double maxY = -std::numeric_limits<double>::infinity();
        for (const auto& ring : rings)
        {
            for (const DevicePoint& point : ring)
            {
                minY = std::min(minY, point.y);
                maxY = std::max(maxY, point.y);
            }
        }
        if (!(minY <= maxY) || rows_ == 0 || columns_ == 0)
        {
            return false;
        }
        const int row0 = static_cast<int>(std::max(0.0, std::ceil(minY / cellSize_ - 0.5)));
        const int row1 = static_cast<int>(std::min(static_cast<double>(rows_ - 1), std::floor(maxY / cellSize_ - 0.5)));
        std::size_t removed = 0;
        std::vector<double> crossings;
        for (int row = row0; row <= row1; ++row)
        {
            const double y = (row + 0.5) * cellSize_;
            crossings.clear();
            for (const auto& ring : rings)
            {
                for (std::size_t i = 0, j = ring.size() - 1; i < ring.size(); j = i++)
                {
                    const DevicePoint& a = ring[i];
                    const DevicePoint& b = ring[j];
                    if ((a.y <= y) != (b.y <= y))
                    {
                        crossings.push_back(a.x + (y - a.y) * (b.x - a.x) / (b.y - a.y));
                    }
                }
            }
            std::ranges::sort(crossings);
            for (std::size_t k = 0; k + 1 < crossings.size(); k += 2)
            {
                const double first = std::max(0.0, std::ceil(crossings[k] / cellSize_ - 0.5));
                const double last = std::min(static_cast<double>(columns_ - 1), std::ceil(crossings[k + 1] / cellSize_ - 0.5) - 1);
                if (first > last)
                {
                    continue;
                }
                const auto rowStart = static_cast<std::size_t>(row) * static_cast<std::size_t>(columns_);
                for (auto column = static_cast<std::size_t>(first); column <= static_cast<std::size_t>(last); ++column)
                {
                    std::uint8_t& cell = cells_[rowStart + column];
                    if (cell == 1)
                    {
                        cell = 0;
                        ++removed;
                    }
                }
            }
        }
        remaining_ -= removed;
        return removed > 0;
    }

    // --- selection -------------------------------------------------------------------------------

    std::optional<SelectableCoverage> makeSelectableCoverage(RecordId dataset, std::optional<int> minimumDisplayScale,
        std::optional<int> optimumDisplayScale, PolygonRings rings)
    {
        if (!optimumDisplayScale || *optimumDisplayScale <= 0)
        {
            return std::nullopt;
        }
        if (minimumDisplayScale && *minimumDisplayScale <= 0)
        {
            minimumDisplayScale.reset();
        }
        const auto bounds = boundsOf(rings);
        if (!bounds)
        {
            return std::nullopt;
        }
        return SelectableCoverage{
            .dataset = dataset,
            .minimumDisplayScale = minimumDisplayScale,
            .optimumDisplayScale = *optimumDisplayScale,
            .bands = scaleBandsOf(minimumDisplayScale, *optimumDisplayScale),
            .rings = std::move(rings),
            .bounds = *bounds,
        };
    }

    bool boundsMeet(const BoundingBox& a, const BoundingBox& b) noexcept
    {
        return a.minX < b.maxX && b.minX < a.maxX && a.minY < b.maxY && b.minY < a.maxY;
    }

    std::vector<SelectedCoverage> selectDataCoverages(std::span<const SelectableCoverage> collection, double scale,
        DeviceRegion& viewport, const DeviceProjection& projection, const std::optional<BoundingBox>& within,
        std::span<const SelectableCoverage* const> pinned)
    {
        std::vector<SelectedCoverage> selected;
        const int displayBand = scaleBandOf(scale);
        std::unordered_map<const SelectableCoverage*, std::vector<std::vector<DevicePoint>>> projected;

        for (const SelectableCoverage* coverage : pinned)
        {
            if (!coverage || coverage->bands.none())
            {
                continue;
            }
            int lowest = 16;
            int highest = 0;
            for (int band = 1; band <= 15; ++band)
            {
                if (coverage->bands.test(static_cast<std::size_t>(band)))
                {
                    lowest = std::min(lowest, band);
                    highest = std::max(highest, band);
                }
            }
            const int band = std::min(std::max(displayBand, lowest), highest);
            const auto& rings = projected.emplace(coverage, project(*coverage, projection)).first->second;
            (void)viewport.subtract(rings);
            selected.push_back(SelectedCoverage{ coverage, band, band < displayBand });
        }

        for (int band = displayBand; band > 0 && !viewport.empty(); --band)
        {
            for (const SelectableCoverage& coverage : collection)
            {
                if (!coverage.bands.test(static_cast<std::size_t>(band)) || std::ranges::find(pinned, &coverage) != pinned.end())
                {
                    continue;
                }
                if (within && !boundsMeet(coverage.bounds, *within))
                {
                    continue;
                }
                auto found = projected.find(&coverage);
                if (found == projected.end())
                {
                    found = projected.emplace(&coverage, project(coverage, projection)).first;
                }
                if (viewport.subtract(found->second))
                {
                    selected.push_back(SelectedCoverage{ &coverage, band, band < displayBand });
                }
            }
        }
        return selected;
    }

    // --- DatasetSelector -------------------------------------------------------------------------

    DatasetSelector::DatasetSelector(std::vector<SelectableCoverage> coverages) : coverages_(std::move(coverages))
    {
    }

    DatasetSelector DatasetSelector::fromDatabase(const SystemDatabase& database)
    {
        std::vector<SelectableCoverage> coverages;
        for (DataCoverageRecord& record : database.coverages().active())
        {
            auto rings = decodeWkbPolygon(record.polygon);
            if (!rings)
            {
                continue;
            }
            if (auto coverage = makeSelectableCoverage(record.datasetId, record.minimumDisplayScale, record.optimumDisplayScale, std::move(*rings)))
            {
                coverages.push_back(std::move(*coverage));
            }
        }
        return DatasetSelector(std::move(coverages));
    }

    std::vector<SelectedCoverage> DatasetSelector::select(const View& view) const
    {
        if (!view.project || !(view.scale > 0) || !(view.width > 0) || !(view.height > 0))
        {
            return {};
        }
        DeviceRegion viewport(view.width, view.height, view.cellSize);
        return selectDataCoverages(coverages_, view.scale, viewport, view.project, view.ground);
    }

    std::vector<RecordId> DatasetSelector::datasetsOf(std::span<const SelectedCoverage> selection)
    {
        std::vector<RecordId> datasets;
        for (const SelectedCoverage& selected : selection)
        {
            if (std::ranges::find(datasets, selected.coverage->dataset) == datasets.end())
            {
                datasets.push_back(selected.coverage->dataset);
            }
        }
        return datasets;
    }
}
