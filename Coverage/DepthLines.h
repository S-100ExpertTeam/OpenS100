#pragma once

#include "CoverageRaster.h"
#include "RegularGrid.h"

#include <cstdint>
#include <functional>
#include <optional>
#include <span>
#include <string_view>
#include <vector>

// The lines S-98 Edition 3.0.0 Appendix D draws over S-102 data: the enhanced safety contour
// (D-1.4), the S-102 coverage boundary (Figure D-1-6) and the water level adjustment boundary
// (D-2.4), and the outline of the cells with data that S-101 depth features are suppressed under
// (opens100-web `core/src/enhanced-safety-contour.ts`, `core/src/valid-grid-rings.ts`,
// `sdk/src/depth-jobs.ts`). Every grid point stands for the cell of one grid spacing around it
// (nearest neighbour, D-1.3 item 3), so all of them are made of the cells' common edges.
namespace s100::coverage
{
    // Line segments as consecutive (lon1, lat1, lon2, lat2) quadruples, in degrees. Consecutive
    // edges along a grid line are one segment (on a UTM grid, as few as still follow the line).
    using GridEdges = std::vector<double>;

    // The enhanced safety contour (D-1.4): edges whose two cells hold depths on either side of
    // `threshold` (shallower is unsafe; depths are positive down). Edges on the grid boundary or
    // next to no data are never selected.
    [[nodiscard]] GridEdges safetyContourEdges(const RegularGrid& grid, std::span<const float> depth, double fillValue, double threshold);

    // Cell classes for boundaryEdgesOfClasses.
    inline constexpr std::uint8_t BoundaryOutside = 0;
    inline constexpr std::uint8_t BoundaryInside = 1;
    inline constexpr std::uint8_t BoundaryInsideUndrawn = 2;

    // The edges where an inside (and drawn) cell meets an outside cell or the grid boundary.
    [[nodiscard]] GridEdges boundaryEdgesOfClasses(const RegularGrid& grid, std::span<const std::uint8_t> classes);
    // The boundary of the cells `inside` holds for; `draw` drops edges by the inside cell they
    // belong to (where the WLA boundary is drawn in the same place instead).
    [[nodiscard]] GridEdges boundaryEdges(const RegularGrid& grid, const std::function<bool(std::size_t)>& inside,
        const std::function<bool(std::size_t)>& draw = {});

    // The S-98 lines of one S-102 grid.
    struct DepthLinesInput
    {
        const RegularGrid* grid = nullptr;
        std::span<const float> depth;       // adjusted when WLA applies
        double fillValue = 0;
        std::span<const std::uint8_t> applied;   // 1 where WLA adjusted the cell; may be empty
        std::size_t appliedCount = 0;
        bool esc = true;
        double safetyContour = 30;
        bool wlaActive = false;
    };

    struct DepthLines
    {
        std::optional<GridEdges> safetyContour;
        std::optional<GridEdges> s102Boundary;
        std::optional<GridEdges> wlaBoundary;
    };

    // With ESC on: the safety contour and the S-102 boundary, which leaves out the cells WLA
    // adjusted while it is active (the WLA boundary is drawn there instead, D-2.4). With WLA
    // active and applied: its boundary.
    [[nodiscard]] DepthLines depthLines(const DepthLinesInput& input);

    // A closed outline of cells with data, its vertices at cell corners; a hole is a ring of its
    // own, so the rings of one grid fill the region under the even-odd rule.
    struct GridRing
    {
        std::vector<GeoPosition> exterior;
    };

    [[nodiscard]] std::vector<GridRing> validGridRings(const RegularGrid& grid, std::span<const float> values, double fillValue);

    // CIE xyL to sRGB with L relative to a white of `whiteL` (80, CHWHT's L in the S-100 colour
    // profiles); out-of-gamut colours are scaled down rather than clipped per channel.
    [[nodiscard]] Rgb xyLToRgb(double x, double y, double L, double whiteL = 80);
    // S-98 D-1 footnote 3: DEPWL, given only as CIE xyL per palette ("Day", "Dusk", "Night").
    [[nodiscard]] Rgb depwlColor(std::string_view palette);

    struct S98LineStyle
    {
        std::string_view token;
        double width = 0;               // millimetres
        std::vector<double> dash;       // alternating dash and gap lengths in millimetres; empty: solid
    };

    // The S-102 coverage boundary (Figure D-1-1), the WLA boundary (Figure D-2-7) and the safety
    // contour as the S-52 double-width DEPSC line (clause 11.6.1).
    [[nodiscard]] const S98LineStyle& safetyContourStyle();
    [[nodiscard]] const S98LineStyle& s102BoundaryStyle();
    [[nodiscard]] const S98LineStyle& wlaBoundaryStyle();
}
