#pragma once

#include "Geo.h"
#include "RegularGrid.h"

#include <functional>
#include <optional>

// Thinning of the arrows drawn at the nodes of a gridded vector field (S-111 surface currents).
// Zoomed out, the nodes of a fine grid come closer together on the screen than their arrows are
// long, and the arrows run over each other. Only every nth row and column is then drawn, with n
// from S-111 Edition 2.0.0 clause 9.3.2 (equations 9.2 and 9.3): the longest arrow may be no
// longer than half the screen distance between diagonal neighbours of the nodes that are drawn.
// Nothing here draws or knows a chart projection; the caller says where a position falls on the
// screen.
namespace s100::coverage
{
    // How the length of an arrow follows the speed: the symbol scaled by speed / referenceSpeed,
    // kept within [minFactor, maxFactor]. The defaults are those of S-111 Edition 2.0.0 clause
    // 9.2.4 (equation 9.1, Table 9-3) and of its portrayal catalogue (Rules/select_arrow.xsl):
    // 10 mm at 5 knots, no shorter than 4 mm (2 knots) and no longer than 26 mm (13 knots).
    struct ArrowScale
    {
        double symbolLength = 10.0;    // length at factor 1, in the unit the caller draws in (mm)
        double referenceSpeed = 5.0;   // the speed drawn at factor 1 (knots)
        double minFactor = 0.4;
        double maxFactor = 2.6;
    };

    // The factor the symbol is scaled by at `speed`; minFactor for a speed that is not a number.
    [[nodiscard]] double arrowScaleFactor(double speed, const ArrowScale& scale = {}) noexcept;
    // The length of the arrow at `speed`, in the unit of scale.symbolLength.
    [[nodiscard]] double arrowLength(double speed, const ArrowScale& scale = {}) noexcept;
    // The longest arrow the scale draws, whatever the speed.
    [[nodiscard]] double maxArrowLength(const ArrowScale& scale = {}) noexcept;

    // R_max of S-111 9.3.2: the longest arrow relative to the diagonal distance between the nodes
    // drawn.
    inline constexpr double MaxArrowToSpacingRatio = 0.5;

    // Every nth row and column is drawn. n is 1 while maxArrowLength / nodeDiagonal is within
    // maxRatio, otherwise 1 + fix(maxArrowLength / (nodeDiagonal * maxRatio)) (equation 9.3).
    // `nodeDiagonal` (D) is the screen distance between a node and its diagonal neighbour and
    // `maxArrowLength` (L_smax) the length of the arrow of the highest speed in the field shown,
    // in the same unit; maxArrowLength(scale) serves when the factor is not to change with the
    // values (from one time to the next). 1 when there is no arrow length to speak of; the largest
    // int when the nodes fall on one point of the screen, which leaves one node.
    [[nodiscard]] int thinningFactor(double nodeDiagonal, double maxArrowLength, double maxRatio = MaxArrowToSpacingRatio) noexcept;

    struct ScreenPoint
    {
        double x = 0;
        double y = 0;
    };
    // Where a geographic position falls on the screen, in the unit arrows are measured in.
    using ToScreen = std::function<ScreenPoint(const GeoPosition&)>;

    // The screen distance between the node nearest `around` and its diagonal neighbour (one column
    // and one row on). `around` is the middle of the grid when not given, and is brought inside
    // the grid when it is outside.
    [[nodiscard]] double nodeDiagonal(const RegularGrid& grid, const ToScreen& toScreen, const std::optional<GeoPosition>& around = {});

    // The nodes left after thinning: columns firstCol, firstCol + factor, ... (`cols` of them) of
    // rows firstRow, firstRow + factor, ... (`rows` of them).
    struct ThinnedNodes
    {
        int factor = 1;
        int firstCol = 0;
        int firstRow = 0;
        int cols = 0;
        int rows = 0;

        [[nodiscard]] bool empty() const noexcept { return cols <= 0 || rows <= 0; }
        [[nodiscard]] std::size_t count() const noexcept
        {
            return empty() ? 0 : static_cast<std::size_t>(cols) * static_cast<std::size_t>(rows);
        }
        // The grid column of the ith column drawn, and the grid row of the jth row drawn.
        [[nodiscard]] int col(int i) const noexcept { return firstCol + i * factor; }
        [[nodiscard]] int row(int j) const noexcept { return firstRow + j * factor; }

        friend bool operator==(const ThinnedNodes&, const ThinnedNodes&) = default;
    };

    // Every `factor`th column and row of the grid (a factor below 1 counts as 1), counted from
    // `anchor`: its column and row are among those drawn. S-111 9.3.2 asks that the row and column
    // of the highest speed are drawn, which is what to pass for that; counted from the grid origin
    // (the default) the same nodes stay as the chart is panned and as time goes on. With `view`,
    // only those in it and up to one drawn node beyond its edges: an arrow is shorter than the
    // distance between the nodes drawn, so none that reaches into the view is left out.
    [[nodiscard]] ThinnedNodes thinnedNodes(const RegularGrid& grid, int factor, const std::optional<GeoBounds>& view = {},
        const GridCell& anchor = {});

    // The nodes to draw arrows at, in one call: the factor from the node spacing on the screen in
    // the middle of the view (of the grid, without a view), then thinnedNodes.
    [[nodiscard]] ThinnedNodes thinArrows(const RegularGrid& grid, const ToScreen& toScreen, double maxArrowLength,
        const std::optional<GeoBounds>& view = {}, const GridCell& anchor = {}, double maxRatio = MaxArrowToSpacingRatio);
}
