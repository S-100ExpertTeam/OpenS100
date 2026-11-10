#pragma once

#include "Geometry.h"
#include "Records.h"

#include <bitset>
#include <cstdint>
#include <functional>
#include <optional>
#include <span>
#include <vector>

// S-98 Appendix E-1 dataset loading: the scale bands (E-1.2) and the Data Coverage selection that
// decides which datasets are drawn for a view (E-1.3). A port of opens100-web
// packages/core/src/dataset-loading.ts; the two give the same selection for the same input (see
// Tests/OpenS100Tests/DatasetSelectionTest.cpp, whose cases are the web tests').
//
// Scales are held as denominators, as the attributes encode them. S-98 compares scales, not
// denominators, so every comparison here is inverted: a scale that is "less than" another has the
// larger denominator.
namespace s100::sdb
{
    // Bands 1 to 15 of S-98 Table 11; bit 0 is unused.
    using ScaleBands = std::bitset<16>;

    // S-98 E-1.2 GetScaleBand: the band (1-15) of a display scale denominator. 1:10,000,000 itself
    // is band 1 (the specification's "scale < 1:10,000,000" leaves it out of bands 1 and 2).
    [[nodiscard]] int scaleBandOf(double scale) noexcept;
    // S-98 E-1.2 scaleBands: the bands a Data Coverage spans. No minimum display scale is 1:infinity.
    [[nodiscard]] ScaleBands scaleBandsOf(std::optional<int> minimumDisplayScale, int optimumDisplayScale) noexcept;

    struct DevicePoint
    {
        double x = 0;
        double y = 0;
    };

    // A device area held as a grid of cells, which is what E-1.3 subtracts coverage footprints
    // from. A cell belongs to a polygon when its centre does, so the polygon operations reduce to
    // scanline fills and stay exact to the cell size.
    class DeviceRegion final
    {
    public:
        // The whole width x height device rectangle.
        DeviceRegion(double width, double height, double cellSize = 4);

        [[nodiscard]] bool empty() const noexcept { return remaining_ == 0; }
        // Cells still in the region.
        [[nodiscard]] std::size_t size() const noexcept { return remaining_; }
        [[nodiscard]] int columns() const noexcept { return columns_; }
        [[nodiscard]] int rows() const noexcept { return rows_; }

        // Removes the polygon (even-odd, so interior rings are holes). Returns whether it
        // overlapped what was left of the region.
        bool subtract(std::span<const std::vector<DevicePoint>> rings);

    private:
        double cellSize_;
        int columns_;
        int rows_;
        std::vector<std::uint8_t> cells_;
        std::size_t remaining_;
    };

    // A Data Coverage taking part in selection.
    struct SelectableCoverage
    {
        RecordId dataset = 0;   // whatever the host identifies the owning dataset by
        std::optional<int> minimumDisplayScale;
        int optimumDisplayScale = 0;
        ScaleBands bands;
        PolygonRings rings;     // longitude/latitude; interior rings are holes
        BoundingBox bounds;
    };

    // std::nullopt for a coverage that cannot take part: no optimum display scale or no polygon.
    [[nodiscard]] std::optional<SelectableCoverage> makeSelectableCoverage(RecordId dataset,
        std::optional<int> minimumDisplayScale, std::optional<int> optimumDisplayScale, PolygonRings rings);

    struct SelectedCoverage
    {
        const SelectableCoverage* coverage = nullptr;
        int scaleBand = 0;         // the band it was selected in
        bool overscaled = false;   // selected below the display band: drawn with OVERSC01 (S-98 11.3.2)
    };

    using DeviceProjection = std::function<DevicePoint(const Position&)>;

    // S-98 E-1.3 SelectDataCoverages. Walks down from the display scale's band, taking every
    // coverage of the band that still overlaps the uncovered viewport and removing its footprint,
    // until the viewport is covered or no band is left. `viewport` is consumed.
    //
    // `within`, when given, is the ground the viewport can show: a coverage wholly outside it is
    // not projected. `pinned` coverages are taken first, whatever the scale and wherever the view
    // is, in the nearest of their bands.
    [[nodiscard]] std::vector<SelectedCoverage> selectDataCoverages(std::span<const SelectableCoverage> collection,
        double scale, DeviceRegion& viewport, const DeviceProjection& projection,
        const std::optional<BoundingBox>& within = std::nullopt, std::span<const SelectableCoverage* const> pinned = {});

    // Whether two extents share area; ones that only touch along an edge, as adjacent cells do, do not.
    [[nodiscard]] bool boundsMeet(const BoundingBox& a, const BoundingBox& b) noexcept;

    class SystemDatabase;

    // The coverages of the installed datasets, read once, and the selection for a view.
    class DatasetSelector final
    {
    public:
        struct View
        {
            double scale = 0;            // display scale denominator
            double width = 0;            // device size
            double height = 0;
            DeviceProjection project;    // longitude/latitude to device
            std::optional<BoundingBox> ground;
            double cellSize = 4;
        };

        DatasetSelector() = default;
        explicit DatasetSelector(std::vector<SelectableCoverage> coverages);
        // The coverages of every active dataset. Reads no dataset file.
        [[nodiscard]] static DatasetSelector fromDatabase(const SystemDatabase& database);

        [[nodiscard]] std::span<const SelectableCoverage> coverages() const noexcept { return coverages_; }

        [[nodiscard]] std::vector<SelectedCoverage> select(const View& view) const;

        // The datasets of a selection, each once, in the order they were first selected (the
        // larger scale first).
        [[nodiscard]] static std::vector<RecordId> datasetsOf(std::span<const SelectedCoverage> selection);

    private:
        std::vector<SelectableCoverage> coverages_;
    };
}
