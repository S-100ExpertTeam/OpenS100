#pragma once

#include "CoverageModel.h"

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <vector>

// Water Level Adjustment (WLA) of S-102 depths by S-104 water levels, with the vertical
// uncertainty treatment, following S-98 Edition 3.0.0 Appendix D: D-1.5 (uncertainty), D-2.2
// (datum compatibility), D-2.5 (Option 1, a single date and time) and D-2.6 (Option 2, a period).
// Option 3, which follows a route schedule, is not implemented (nor is it in the web port this is
// taken from, opens100-web `core/src/water-level-adjustment.ts`).
namespace s100::coverage
{
    struct WlaMethod
    {
        enum class Kind
        {
            Off,
            Instant,   // Option 1 (D-2.5)
            Period,    // Option 2 (D-2.6)
        };
        Kind kind = Kind::Off;
        TimePoint time{};    // Instant
        TimePoint start{};   // Period, in either order
        TimePoint end{};

        [[nodiscard]] static WlaMethod off() { return {}; }
        [[nodiscard]] static WlaMethod instant(TimePoint at) { return { Kind::Instant, at, {}, {} }; }
        [[nodiscard]] static WlaMethod period(TimePoint from, TimePoint to) { return { Kind::Period, {}, from, to }; }
    };

    enum class WlaIssue
    {
        IncompatibleDatums,
        TimeNotCovered,
    };

    struct DepthAdjustmentSettings
    {
        bool uncertainty = false;   // the Uncertainty selector (D-1.5, D-2 item 3f)
        WlaMethod wla;
        // Applies WLA across differing datums anyway. Not permitted by D-2.2 item 5; for test data only.
        bool ignoreIncompatibleDatums = false;
    };

    struct DepthAdjustment
    {
        // Depth used for portrayal and the safety contour: uncertainty and WLA applied, fill values unchanged.
        std::shared_ptr<const std::vector<float>> depth;
        std::vector<float> waterLevel;      // added to each cell, positive up; NaN where WLA was not applied
        std::vector<std::uint8_t> applied;  // 1 where WLA was applied
        std::size_t appliedCount = 0;
    };

    struct WlaReport
    {
        std::size_t applied = 0;   // cells adjusted by WLA
        std::vector<WlaIssue> issues;

        friend bool operator==(const WlaReport&, const WlaReport&) = default;
    };

    // The S-104 records a WLA method draws on, or std::nullopt when the series does not cover the
    // time (D-2.4). Option 1 takes the record at the instant, or the two either side of it;
    // Option 2 the records within the period plus the ones immediately before and after it.
    [[nodiscard]] std::optional<std::vector<std::size_t>> selectWaterLevelRecords(std::span<const CoverageTimeGroup> groups, const WlaMethod& method);

    struct VerticalDatum
    {
        bool epsg = false;   // the S-100 list (verticalDatumReference 1) otherwise
        int value = 0;

        friend bool operator==(const VerticalDatum&, const VerticalDatum&) = default;
    };

    // The dataset's root vertical datum; std::nullopt when it states none.
    [[nodiscard]] std::optional<VerticalDatum> verticalDatumOf(const CoverageDataset& dataset);
    // "approximateMeanLowerLowWater (15)", "EPSG:5703", "unknown datum".
    [[nodiscard]] std::string verticalDatumName(const std::optional<VerticalDatum>& datum);
    // +1 when water levels are heights (EPSG 6499, positive up), -1 when depths (6498, positive
    // down), per S-104 Table 12-1 and EPSG (S-98 D-1.5 footnote 5 states the codes the other way round).
    [[nodiscard]] int waterLevelSign(const CoverageDataset& dataset);
    // Whether two coverages overlap by more than one grid cell (D-1.3 item 2, D-2.2 item 1).
    [[nodiscard]] bool coveragesOverlap(const CoverageDataset& a, const CoverageDataset& b);
    // "hh:mm dd Mmm yyyy" (D-2.5, D-3.2), UTC.
    [[nodiscard]] std::string formatWlaDateTime(TimePoint time);
    // The permanent indication of D-2.5 / D-2.6, "WLA 12:34 08 Nov 2021"; std::nullopt when off.
    [[nodiscard]] std::optional<std::string> wlaIndication(const WlaMethod& method);

    // Each S-104 cell's shoalest selected water level, positive up, uncertainty applied; NaN
    // where any selected record has no value (D-2.6).
    struct WaterLevelGrid
    {
        RegularGrid grid;
        std::vector<float> levels;
    };

    // What adjustDepths works from: one S-102 feature's raw grid and the water level grids over it.
    struct DepthAdjustmentInput
    {
        const RegularGrid* grid = nullptr;
        std::span<const float> raw;
        double fillValue = 0;
        std::span<const float> uncertainties;        // empty: not applied
        std::optional<double> uncertaintyFill;
        std::span<const WaterLevelGrid> grids;
    };

    // One feature's adjusted depths (D-1.5, D-2.5): the shoalest water level among the S-104
    // cells whose extents intersect each cell's (Figure D-2-9) added, the uncertainty taken off.
    [[nodiscard]] DepthAdjustment adjustDepths(const DepthAdjustmentInput& input);

    // An S-102 coverage whose depth values are adjusted: give it to the portrayal in place of the
    // S-102 source so depth shading and the safety contour use the adjusted depths (D-2 item 3a);
    // base() keeps the raw values for the pick report.
    class WaterLevelAdjustedCoverage final : public CoverageSource
    {
    public:
        explicit WaterLevelAdjustedCoverage(std::shared_ptr<const CoverageSource> base);

        [[nodiscard]] const CoverageDataset& dataset() const noexcept override { return base_->dataset(); }
        [[nodiscard]] Values readValues(std::string_view featureId, const CoverageSelection& selection) const override;
        [[nodiscard]] std::uint64_t revision() const noexcept override { return version_; }

        [[nodiscard]] const CoverageSource& base() const noexcept { return *base_; }
        [[nodiscard]] const DepthAdjustment* adjustment(std::string_view featureId) const;

        // Recomputes the adjusted depths against `waterLevels` (S-104 datasets).
        WlaReport update(const DepthAdjustmentSettings& settings, std::span<const CoverageSource* const> waterLevels);

    private:
        std::shared_ptr<const CoverageSource> base_;
        std::map<std::string, DepthAdjustment, std::less<>> adjustments_;
        std::uint64_t version_ = 0;
        bool plain_ = false;   // the adjustments are the raw depths untouched
    };
}
