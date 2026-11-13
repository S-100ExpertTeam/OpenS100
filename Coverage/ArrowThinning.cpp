#include "ArrowThinning.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace s100::coverage
{
    namespace
    {
        // The nodes lo..hi (inclusive, lo >= 0) reduced to those `offset` beyond a multiple of
        // `factor` (0 <= offset < factor): the first, and how many.
        struct Run
        {
            int first = 0;
            int count = 0;
        };

        Run multiplesWithin(long long lo, long long hi, long long factor, long long offset) noexcept
        {
            if (lo > hi)
            {
                return {};
            }
            const long long first = lo <= offset ? offset : offset + ((lo - offset + factor - 1) / factor) * factor;
            if (first > hi)
            {
                return {};
            }
            return Run{ static_cast<int>(first), static_cast<int>((hi - first) / factor + 1) };
        }

        // The whole nodes an interval of fractional indices takes in, widened by `margin` nodes and
        // cut to the grid's 0..size-1. Empty (lo > hi) when the interval misses the grid.
        std::pair<long long, long long> nodeRange(double from, double to, long long margin, int size) noexcept
        {
            const double last = static_cast<double>(size) - 1;
            const double lo = std::min(from, to);
            const double hi = std::max(from, to);
            if (!(hi >= -0.5 && lo <= last + 0.5))   // also when either is not a number
            {
                return { 1, 0 };
            }
            const long long first = static_cast<long long>(std::floor(std::max(lo, 0.0))) - margin;
            const long long end = static_cast<long long>(std::ceil(std::min(hi, last))) + margin;
            return { std::max(first, 0LL), std::min(end, static_cast<long long>(size) - 1) };
        }
    }

    double arrowScaleFactor(double speed, const ArrowScale& scale) noexcept
    {
        if (!(speed > 0) || !(scale.referenceSpeed > 0))
        {
            return scale.minFactor;
        }
        return std::clamp(speed / scale.referenceSpeed, scale.minFactor, std::max(scale.minFactor, scale.maxFactor));
    }

    double arrowLength(double speed, const ArrowScale& scale) noexcept
    {
        return scale.symbolLength * arrowScaleFactor(speed, scale);
    }

    double maxArrowLength(const ArrowScale& scale) noexcept
    {
        return scale.symbolLength * std::max(scale.minFactor, scale.maxFactor);
    }

    int thinningFactor(double nodeDiagonal, double maxArrowLength, double maxRatio) noexcept
    {
        constexpr int largest = std::numeric_limits<int>::max();
        if (!(maxArrowLength > 0) || !(maxRatio > 0))
        {
            return 1;
        }
        if (!(nodeDiagonal > 0))
        {
            return largest;
        }
        const double ratio = maxArrowLength / (maxRatio * nodeDiagonal);
        if (ratio <= 1)
        {
            return 1;   // equation 9.2 holds with every node drawn
        }
        const double factor = 1 + std::floor(ratio);
        return factor < static_cast<double>(largest) ? static_cast<int>(factor) : largest;
    }

    double nodeDiagonal(const RegularGrid& grid, const ToScreen& toScreen, const std::optional<GeoPosition>& around)
    {
        if (grid.cols <= 0 || grid.rows <= 0 || !toScreen)
        {
            return 0;
        }
        // The node whose diagonal neighbour is still on the grid; a grid of one row or one column
        // has none, and is measured to where the neighbour would be.
        double col = (grid.cols - 1) / 2;
        double row = (grid.rows - 1) / 2;
        if (around)
        {
            const GridIndex at = gridCoordinates(grid, *around);
            if (std::isfinite(at.col) && std::isfinite(at.row))
            {
                col = std::floor(at.col);
                row = std::floor(at.row);
            }
        }
        col = std::clamp(col, 0.0, static_cast<double>(std::max(grid.cols - 2, 0)));
        row = std::clamp(row, 0.0, static_cast<double>(std::max(grid.rows - 2, 0)));
        const ScreenPoint here = toScreen(gridPosition(grid, col, row));
        const ScreenPoint there = toScreen(gridPosition(grid, col + 1, row + 1));
        return std::hypot(there.x - here.x, there.y - here.y);
    }

    ThinnedNodes thinnedNodes(const RegularGrid& grid, int factor, const std::optional<GeoBounds>& view, const GridCell& anchor)
    {
        ThinnedNodes nodes{ .factor = std::max(factor, 1) };
        if (grid.cols <= 0 || grid.rows <= 0)
        {
            return nodes;
        }
        std::pair<long long, long long> colRange{ 0, grid.cols - 1 };
        std::pair<long long, long long> rowRange{ 0, grid.rows - 1 };
        if (view)
        {
            // The view's corners in node indices; on a UTM grid the view is not a rectangle of
            // nodes, and the range that holds all four corners is taken.
            double minCol = std::numeric_limits<double>::infinity();
            double maxCol = -minCol;
            double minRow = minCol;
            double maxRow = -minCol;
            for (const GeoPosition& corner : { GeoPosition{ view->minLon, view->minLat }, GeoPosition{ view->maxLon, view->minLat },
                     GeoPosition{ view->minLon, view->maxLat }, GeoPosition{ view->maxLon, view->maxLat } })
            {
                const GridIndex at = gridCoordinates(grid, corner);
                minCol = std::min(minCol, at.col);
                maxCol = std::max(maxCol, at.col);
                minRow = std::min(minRow, at.row);
                maxRow = std::max(maxRow, at.row);
            }
            colRange = nodeRange(minCol, maxCol, nodes.factor, grid.cols);
            rowRange = nodeRange(minRow, maxRow, nodes.factor, grid.rows);
        }
        // The anchor's place within a step, also for an anchor before the origin.
        const long long step = nodes.factor;
        const auto offset = [step](long long index) { return ((index % step) + step) % step; };
        const Run cols = multiplesWithin(colRange.first, colRange.second, nodes.factor, offset(anchor.col));
        const Run rows = multiplesWithin(rowRange.first, rowRange.second, nodes.factor, offset(anchor.row));
        if (cols.count <= 0 || rows.count <= 0)
        {
            return nodes;
        }
        nodes.firstCol = cols.first;
        nodes.cols = cols.count;
        nodes.firstRow = rows.first;
        nodes.rows = rows.count;
        return nodes;
    }

    ThinnedNodes thinArrows(const RegularGrid& grid, const ToScreen& toScreen, double maxArrowLength,
        const std::optional<GeoBounds>& view, const GridCell& anchor, double maxRatio)
    {
        const std::optional<GeoPosition> middle = view
            ? std::optional<GeoPosition>(GeoPosition{ (view->minLon + view->maxLon) / 2, (view->minLat + view->maxLat) / 2 })
            : std::nullopt;
        return thinnedNodes(grid, thinningFactor(nodeDiagonal(grid, toScreen, middle), maxArrowLength, maxRatio), view, anchor);
    }
}
