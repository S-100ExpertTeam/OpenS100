#include "WaterLevelAdjustment.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <format>
#include <limits>
#include <set>

namespace s100::coverage
{
    namespace
    {
        constexpr float NaN = std::numeric_limits<float>::quiet_NaN();

        // Slack, in cells: a target cell's edge that lands on a source cell's edge lands a few
        // ulps past it with a spacing like 0.01 degrees, which would count the neighbour that
        // only touches there as intersecting.
        constexpr double EdgeSlack = 1e-6;

        bool inRange(float value, const CoverageComponent& component) noexcept
        {
            return !isNoData(value, component.fillValue) && (!component.lower || value >= *component.lower)
                && (!component.upper || value <= *component.upper);
        }

        // An uncertainty, or 0 when it is missing, negative or a fill value (D-1.5).
        float uncertaintyOf(float value, const CoverageComponent& component) noexcept
        {
            return !isNoData(value, component.fillValue) && value >= 0 ? value : 0.0f;
        }

        bool sameFill(float value, double fill) noexcept
        {
            return std::isnan(fill) ? std::isnan(value) : static_cast<double>(value) == fill;
        }

        // Indices of the source cells whose extents strictly intersect [center - half, center + half].
        std::pair<int, int> intersectingRange(double center, double half, double origin, double spacing, int count) noexcept
        {
            const double sourceHalf = spacing / 2;
            return {
                std::max(0, static_cast<int>(std::floor((center - half - origin - sourceHalf) / spacing + EdgeSlack)) + 1),
                std::min(count - 1, static_cast<int>(std::ceil((center + half - origin + sourceHalf) / spacing - EdgeSlack)) - 1),
            };
        }

        bool sameGridCrs(const RegularGrid& a, const RegularGrid& b) noexcept
        {
            return a.utm == b.utm;
        }

        // Lowers each target cell's level to the shoalest among the source cells whose extents
        // intersect its extent: NaN is skipped, a NaN so far is replaced.
        void accumulateShoalest(const RegularGrid& target, const WaterLevelGrid& source, std::vector<float>& out)
        {
            const RegularGrid& s = source.grid;
            const auto lower = [&](std::size_t k, float shoalest) {
                if (!std::isnan(shoalest) && !(out[k] <= shoalest)) out[k] = shoalest;
            };
            if (sameGridCrs(target, s))
            {
                std::vector<std::pair<int, int>> cols(static_cast<std::size_t>(target.cols));
                std::vector<std::pair<int, int>> rows(static_cast<std::size_t>(target.rows));
                for (int c = 0; c < target.cols; ++c)
                {
                    cols[static_cast<std::size_t>(c)] = intersectingRange(target.originLon + c * target.spacingLon, std::abs(target.spacingLon) / 2,
                        s.originLon, s.spacingLon, s.cols);
                }
                for (int r = 0; r < target.rows; ++r)
                {
                    rows[static_cast<std::size_t>(r)] = intersectingRange(target.originLat + r * target.spacingLat, std::abs(target.spacingLat) / 2,
                        s.originLat, s.spacingLat, s.rows);
                }
                for (int r = 0; r < target.rows; ++r)
                {
                    const auto [i0, i1] = rows[static_cast<std::size_t>(r)];
                    if (i0 > i1) continue;
                    for (int c = 0; c < target.cols; ++c)
                    {
                        const auto [j0, j1] = cols[static_cast<std::size_t>(c)];
                        if (j0 > j1) continue;
                        float shoalest = NaN;
                        for (int i = i0; i <= i1; ++i)
                        {
                            for (int j = j0; j <= j1; ++j)
                            {
                                const float v = source.levels[static_cast<std::size_t>(i) * static_cast<std::size_t>(s.cols) + static_cast<std::size_t>(j)];
                                if (!std::isnan(v) && !(v >= shoalest)) shoalest = v;
                            }
                        }
                        lower(static_cast<std::size_t>(r) * static_cast<std::size_t>(target.cols) + static_cast<std::size_t>(c), shoalest);
                    }
                }
                return;
            }

            // Grids on different CRSs, whose cells are not parallel: a target cell's extent is
            // the box around its four corners in the source grid, which can only add a source
            // cell it touches at a corner (the shoaler, safe side).
            LatticeMapping corners(target.cols + 1, target.rows + 1,
                [&](double x, double y) { return gridPosition(target, x - 0.5, y - 0.5); }, s);
            const auto n = static_cast<std::size_t>(target.cols) + 1;
            std::vector<double> col0(n), row0(n), col1(n), row1(n);
            corners.line(0, col0, row0);
            for (int r = 0; r < target.rows; ++r)
            {
                corners.line(r + 1, col1, row1);
                for (int c = 0; c < target.cols; ++c)
                {
                    const auto k = static_cast<std::size_t>(c);
                    const double minCol = std::min({ col0[k], col0[k + 1], col1[k], col1[k + 1] });
                    const double maxCol = std::max({ col0[k], col0[k + 1], col1[k], col1[k + 1] });
                    const double minRow = std::min({ row0[k], row0[k + 1], row1[k], row1[k + 1] });
                    const double maxRow = std::max({ row0[k], row0[k + 1], row1[k], row1[k + 1] });
                    const int j0 = std::max(0, static_cast<int>(std::floor(minCol - 0.5 + EdgeSlack)) + 1);
                    const int j1 = std::min(s.cols - 1, static_cast<int>(std::ceil(maxCol + 0.5 - EdgeSlack)) - 1);
                    const int i0 = std::max(0, static_cast<int>(std::floor(minRow - 0.5 + EdgeSlack)) + 1);
                    const int i1 = std::min(s.rows - 1, static_cast<int>(std::ceil(maxRow + 0.5 - EdgeSlack)) - 1);
                    float shoalest = NaN;
                    for (int i = i0; i <= i1; ++i)
                    {
                        for (int j = j0; j <= j1; ++j)
                        {
                            const float v = source.levels[static_cast<std::size_t>(i) * static_cast<std::size_t>(s.cols) + static_cast<std::size_t>(j)];
                            if (!std::isnan(v) && !(v >= shoalest)) shoalest = v;
                        }
                    }
                    lower(static_cast<std::size_t>(r) * static_cast<std::size_t>(target.cols) + k, shoalest);
                }
                std::swap(col0, col1);
                std::swap(row0, row1);
            }
        }

        struct SelectedGrids
        {
            std::vector<WaterLevelGrid> grids;
            bool timeNotCovered = false;
        };

        SelectedGrids waterLevelGrids(const CoverageSource& source, const WlaMethod& method, bool uncertainty)
        {
            SelectedGrids result;
            bool uncovered = false;
            const int sign = waterLevelSign(source.dataset());
            for (const CoverageFeature& feature : source.dataset().features)
            {
                if (feature.code != "WaterLevel") continue;
                const auto height = feature.componentIndex("waterLevelHeight");
                if (!height) continue;
                const auto records = selectWaterLevelRecords(feature.timeGroups, method);
                if (!records)
                {
                    uncovered = true;
                    continue;
                }
                const CoverageComponent& heightComponent = feature.components[*height];
                const auto uncertaintyIndex = uncertainty ? feature.componentIndex("uncertainty") : std::nullopt;
                const std::size_t cells = feature.grid.nodeCount();
                std::vector<float> levels(cells, std::numeric_limits<float>::infinity());
                for (const std::size_t timeIndex : *records)
                {
                    const Values values = source.readValues(feature.id, { .timeIndex = timeIndex, .component = *height });
                    const Values uncertainties = uncertaintyIndex ? source.readValues(feature.id, { .timeIndex = timeIndex, .component = *uncertaintyIndex }) : nullptr;
                    for (std::size_t i = 0; i < cells; ++i)
                    {
                        const float value = values && i < values->size() ? (*values)[i] : NaN;
                        if (!inRange(value, heightComponent))
                        {
                            levels[i] = NaN;
                            continue;
                        }
                        if (std::isnan(levels[i])) continue;
                        const float u = uncertainties && i < uncertainties->size()
                            ? uncertaintyOf((*uncertainties)[i], feature.components[*uncertaintyIndex]) : 0.0f;
                        levels[i] = std::min(levels[i], static_cast<float>(sign) * value - u);
                    }
                }
                for (float& level : levels)
                {
                    if (std::isinf(level)) level = NaN;
                }
                result.grids.push_back({ feature.grid, std::move(levels) });
            }
            result.timeNotCovered = result.grids.empty() && uncovered;
            return result;
        }

        // The horizontal datum's geographic CRS: WGS 84 / UTM grids are on EPSG:4326's datum.
        int horizontalCrs(const CoverageDataset& dataset)
        {
            const int code = static_cast<int>(dataset.numberAttribute("horizontalCRS").value_or(4326));
            return utmZoneFromEpsg(code) ? 4326 : code;
        }

        DepthAdjustment plainAdjustment(const Values& raw)
        {
            const std::size_t count = raw ? raw->size() : 0;
            return DepthAdjustment{ raw, std::vector<float>(count, NaN), std::vector<std::uint8_t>(count, 0), 0 };
        }
    }

    std::optional<std::vector<std::size_t>> selectWaterLevelRecords(std::span<const CoverageTimeGroup> groups, const WlaMethod& method)
    {
        if (groups.empty() || method.kind == WlaMethod::Kind::Off
            || std::ranges::any_of(groups, [](const CoverageTimeGroup& group) { return !group.time; }))
        {
            return std::nullopt;
        }
        const TimePoint first = *groups.front().time;
        const TimePoint last = *groups.back().time;
        if (method.kind == WlaMethod::Kind::Instant)
        {
            if (method.time < first || method.time > last) return std::nullopt;
            for (std::size_t i = 0; i < groups.size(); ++i)
            {
                if (*groups[i].time == method.time) return std::vector<std::size_t>{ i };
            }
            for (std::size_t i = 0; i < groups.size(); ++i)
            {
                if (*groups[i].time > method.time) return std::vector<std::size_t>{ i - 1, i };
            }
            return std::nullopt;
        }
        const TimePoint start = std::min(method.start, method.end);
        const TimePoint end = std::max(method.start, method.end);
        if (start < first || end > last) return std::nullopt;
        std::size_t from = 0;
        for (std::size_t i = 0; i < groups.size(); ++i)
        {
            if (*groups[i].time <= start) from = i;
        }
        std::size_t to = groups.size() - 1;
        for (std::size_t i = groups.size(); i-- > 0;)
        {
            if (*groups[i].time >= end) to = i;
        }
        std::vector<std::size_t> records;
        for (std::size_t i = from; i <= to; ++i) records.push_back(i);
        return records;
    }

    std::optional<VerticalDatum> verticalDatumOf(const CoverageDataset& dataset)
    {
        const auto value = dataset.numberAttribute("verticalDatum");
        if (!value)
        {
            return std::nullopt;
        }
        bool epsg = false;
        if (const auto number = dataset.numberAttribute("verticalDatumReference"))
        {
            epsg = *number == 2;
        }
        else if (auto text = dataset.textAttribute("verticalDatumReference"))
        {
            std::ranges::transform(*text, text->begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            epsg = *text == "2" || *text == "epsg";
        }
        return VerticalDatum{ epsg, static_cast<int>(*value) };
    }

    std::string verticalDatumName(const std::optional<VerticalDatum>& datum)
    {
        if (!datum)
        {
            return "unknown datum";
        }
        if (datum->epsg)
        {
            return std::format("EPSG:{}", datum->value);
        }
        static const std::map<int, const char*> names{
            { 1, "meanLowWaterSprings" }, { 2, "meanLowerLowWaterSprings" }, { 3, "meanSeaLevel" }, { 4, "lowestLowWater" },
            { 5, "meanLowWater" }, { 6, "lowestLowWaterSprings" }, { 7, "approximateMeanLowWaterSprings" }, { 8, "indianSpringLowWater" },
            { 9, "lowWaterSprings" }, { 10, "approximateLowestAstronomicalTide" }, { 11, "nearlyLowestLowWater" }, { 12, "meanLowerLowWater" },
            { 13, "lowWater" }, { 14, "approximateMeanLowWater" }, { 15, "approximateMeanLowerLowWater" }, { 16, "meanHighWater" },
            { 17, "meanHighWaterSprings" }, { 18, "highWater" }, { 19, "approximateMeanSeaLevel" }, { 20, "highWaterSprings" },
            { 21, "meanHigherHighWater" }, { 22, "equinoctialSpringLowWater" }, { 23, "lowestAstronomicalTide" }, { 24, "localDatum" },
            { 25, "internationalGreatLakesDatum1985" }, { 26, "meanWaterLevel" }, { 27, "lowerLowWaterLargeTide" },
            { 28, "higherHighWaterLargeTide" }, { 29, "nearlyHighestHighWater" }, { 30, "highestAstronomicalTide" },
            { 44, "balticSeaChartDatum2000" }, { 46, "internationalGreatLakesDatum2020" },
        };
        const auto found = names.find(datum->value);
        return std::format("{} ({})", found != names.end() ? found->second : "unknown", datum->value);
    }

    int waterLevelSign(const CoverageDataset& dataset)
    {
        return dataset.numberAttribute("verticalCS") == 6498 ? -1 : 1;
    }

    bool coveragesOverlap(const CoverageDataset& a, const CoverageDataset& b)
    {
        double cell = 0;
        for (const CoverageDataset* dataset : { &a, &b })
        {
            for (const CoverageFeature& feature : dataset->features)
            {
                const CellSize size = gridCellSize(feature.grid);
                cell = std::max({ cell, size.lon, size.lat });
            }
        }
        const double width = std::min(a.bounds.maxLon, b.bounds.maxLon) - std::max(a.bounds.minLon, b.bounds.minLon);
        const double height = std::min(a.bounds.maxLat, b.bounds.maxLat) - std::max(a.bounds.minLat, b.bounds.minLat);
        return width > cell && height > cell;
    }

    std::string formatWlaDateTime(TimePoint time)
    {
        static constexpr std::array<const char*, 12> months{ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
        using namespace std::chrono;
        const auto day = floor<days>(time);
        const year_month_day date{ day };
        const hh_mm_ss clock{ floor<minutes>(time - day) };
        return std::format("{:02}:{:02} {:02} {} {}", clock.hours().count(), clock.minutes().count(), static_cast<unsigned>(date.day()),
            months[static_cast<unsigned>(date.month()) - 1], static_cast<int>(date.year()));
    }

    std::optional<std::string> wlaIndication(const WlaMethod& method)
    {
        switch (method.kind)
        {
        case WlaMethod::Kind::Instant:
            return "WLA " + formatWlaDateTime(method.time);
        case WlaMethod::Kind::Period:
            return "WLA from " + formatWlaDateTime(std::min(method.start, method.end)) + " to " + formatWlaDateTime(std::max(method.start, method.end));
        default:
            return std::nullopt;
        }
    }

    DepthAdjustment adjustDepths(const DepthAdjustmentInput& input)
    {
        const std::size_t cells = input.raw.size();
        std::vector<float> waterLevel(cells, NaN);
        if (input.grid)
        {
            for (const WaterLevelGrid& level : input.grids)
            {
                accumulateShoalest(*input.grid, level, waterLevel);
            }
        }
        auto depth = std::make_shared<std::vector<float>>(cells);
        std::vector<std::uint8_t> mask(cells, 0);
        std::size_t count = 0;
        const double uncertaintyFill = input.uncertaintyFill.value_or(std::numeric_limits<double>::quiet_NaN());
        for (std::size_t i = 0; i < cells; ++i)
        {
            const float value = input.raw[i];
            if (sameFill(value, input.fillValue))
            {
                (*depth)[i] = value;
                waterLevel[i] = NaN;
                continue;
            }
            // Depth is positive down, so taking the uncertainty off biases towards safety.
            float u = 0;
            if (i < input.uncertainties.size())
            {
                const float raw = input.uncertainties[i];
                if (!sameFill(raw, uncertaintyFill) && raw >= 0) u = raw;
            }
            const float level = waterLevel[i];
            if (std::isnan(level))
            {
                (*depth)[i] = value - u;
            }
            else
            {
                (*depth)[i] = value - u + level;
                mask[i] = 1;
                ++count;
            }
        }
        return DepthAdjustment{ std::move(depth), std::move(waterLevel), std::move(mask), count };
    }

    WaterLevelAdjustedCoverage::WaterLevelAdjustedCoverage(std::shared_ptr<const CoverageSource> base)
        : base_(std::move(base))
    {
        (void)update({}, {});
    }

    const DepthAdjustment* WaterLevelAdjustedCoverage::adjustment(std::string_view featureId) const
    {
        const auto found = adjustments_.find(featureId);
        return found == adjustments_.end() ? nullptr : &found->second;
    }

    Values WaterLevelAdjustedCoverage::readValues(std::string_view featureId, const CoverageSelection& selection) const
    {
        const auto found = adjustments_.find(featureId);
        const CoverageFeature* feature = base_->dataset().feature(featureId);
        if (found != adjustments_.end() && feature && selection.component < feature->components.size()
            && feature->components[selection.component].name == "depth" && selection.timeIndex == 0)
        {
            return found->second.depth;
        }
        return base_->readValues(featureId, selection);
    }

    WlaReport WaterLevelAdjustedCoverage::update(const DepthAdjustmentSettings& settings, std::span<const CoverageSource* const> waterLevels)
    {
        std::set<WlaIssue> issues;
        std::vector<WaterLevelGrid> grids;
        const CoverageDataset& s102 = base_->dataset();
        if (settings.wla.kind != WlaMethod::Kind::Off)
        {
            const auto datum = verticalDatumOf(s102);
            for (const CoverageSource* source : waterLevels)
            {
                if (!source || !source->dataset().bounds.intersects(s102.bounds)) continue;
                // D-2.2 item 5: WLA needs the same vertical and horizontal datum.
                const auto otherDatum = verticalDatumOf(source->dataset());
                if (!datum || !otherDatum || *datum != *otherDatum || horizontalCrs(source->dataset()) != horizontalCrs(s102))
                {
                    issues.insert(WlaIssue::IncompatibleDatums);
                    if (!settings.ignoreIncompatibleDatums) continue;
                }
                SelectedGrids selected = waterLevelGrids(*source, settings.wla, settings.uncertainty);
                if (selected.timeNotCovered)
                {
                    issues.insert(WlaIssue::TimeNotCovered);
                }
                std::ranges::move(selected.grids, std::back_inserter(grids));
            }
        }
        WlaReport report{ 0, std::vector<WlaIssue>(issues.begin(), issues.end()) };

        // With no water level to add and no uncertainty to take off, the adjusted depth is the raw
        // one: shared rather than copied, and a second such run leaves the revision alone.
        const bool plain = grids.empty() && !settings.uncertainty;
        if (plain && plain_ && !adjustments_.empty())
        {
            return report;
        }
        plain_ = plain;

        adjustments_.clear();
        for (const CoverageFeature& feature : s102.features)
        {
            const auto depthIndex = feature.componentIndex("depth");
            if (!depthIndex) continue;
            const Values raw = base_->readValues(feature.id, { .timeIndex = 0, .component = *depthIndex });
            if (!raw) continue;
            DepthAdjustment adjustment;
            if (plain)
            {
                adjustment = plainAdjustment(raw);
            }
            else
            {
                const auto uncertaintyIndex = settings.uncertainty ? feature.componentIndex("uncertainty") : std::nullopt;
                const Values uncertainties = uncertaintyIndex ? base_->readValues(feature.id, { .timeIndex = 0, .component = *uncertaintyIndex }) : nullptr;
                adjustment = adjustDepths({ .grid = &feature.grid, .raw = *raw, .fillValue = feature.components[*depthIndex].fillValue,
                    .uncertainties = uncertainties ? std::span<const float>(*uncertainties) : std::span<const float>(),
                    .uncertaintyFill = uncertaintyIndex ? std::optional(feature.components[*uncertaintyIndex].fillValue) : std::nullopt,
                    .grids = grids });
            }
            report.applied += adjustment.appliedCount;
            adjustments_.emplace(feature.id, std::move(adjustment));
        }
        ++version_;
        return report;
    }
}
