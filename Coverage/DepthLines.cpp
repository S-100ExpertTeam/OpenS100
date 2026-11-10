#include "DepthLines.h"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>

namespace s100::coverage
{
    namespace
    {
        using EdgeTable = std::array<std::uint8_t, 16>;

        // A 4 x 4 table with 1 where the unordered pair of classes is listed.
        EdgeTable edgeTable(std::initializer_list<std::pair<int, int>> pairs)
        {
            EdgeTable table{};
            for (const auto& [a, b] : pairs)
            {
                table[static_cast<std::size_t>(a * 4 + b)] = 1;
                table[static_cast<std::size_t>(b * 4 + a)] = 1;
            }
            return table;
        }

        // The cell edges between cells of different classes where `table` says so, class 0 being
        // beyond the grid; consecutive selected edges along a grid line are merged.
        GridEdges collectEdges(const RegularGrid& grid, std::span<const std::uint8_t> classes, const EdgeTable& table)
        {
            const int cols = grid.cols;
            const int rows = grid.rows;
            GridEdges out;
            // Edge line k lies between column (or row) k-1 and k. A geographic grid's edges are
            // straight on the chart; a projected grid's are bent by gridLine.
            const auto push = [&](int c0, int r0, int c1, int r1) {
                if (grid.utm)
                {
                    const auto line = gridLine(grid, c0 - 0.5, r0 - 0.5, c1 - 0.5, r1 - 0.5);
                    for (std::size_t i = 1; i < line.size(); ++i)
                    {
                        out.insert(out.end(), { line[i - 1].lon, line[i - 1].lat, line[i].lon, line[i].lat });
                    }
                    return;
                }
                out.insert(out.end(), { grid.originLon + (c0 - 0.5) * grid.spacingLon, grid.originLat + (r0 - 0.5) * grid.spacingLat,
                    grid.originLon + (c1 - 0.5) * grid.spacingLon, grid.originLat + (r1 - 0.5) * grid.spacingLat });
            };
            const auto cls = [&](int row, int col) -> int { return classes[static_cast<std::size_t>(row) * static_cast<std::size_t>(cols) + static_cast<std::size_t>(col)]; };

            for (int r = 0; r <= rows; ++r)
            {
                int start = -1;
                for (int c = 0; c <= cols; ++c)
                {
                    const bool on = c < cols && table[static_cast<std::size_t>((r > 0 ? cls(r - 1, c) : 0) * 4 + (r < rows ? cls(r, c) : 0))] == 1;
                    if (on && start < 0) start = c;
                    if (!on && start >= 0)
                    {
                        push(start, r, c, r);
                        start = -1;
                    }
                }
            }
            for (int c = 0; c <= cols; ++c)
            {
                int start = -1;
                for (int r = 0; r <= rows; ++r)
                {
                    const bool on = r < rows && table[static_cast<std::size_t>((c > 0 ? cls(r, c - 1) : 0) * 4 + (c < cols ? cls(r, c) : 0))] == 1;
                    if (on && start < 0) start = r;
                    if (!on && start >= 0)
                    {
                        push(c, start, c, r);
                        start = -1;
                    }
                }
            }
            return out;
        }

        // Exact fill comparison, NaN-aware, as the web's depthLines does.
        bool isFill(float value, double fillValue) noexcept
        {
            return std::isnan(fillValue) ? std::isnan(value) : static_cast<double>(value) == fillValue;
        }
    }

    GridEdges safetyContourEdges(const RegularGrid& grid, std::span<const float> depth, double fillValue, double threshold)
    {
        // 1 unsafe (shallower than the threshold), 2 safe, 0 no data: an edge lies between 1 and 2.
        std::vector<std::uint8_t> classes(grid.nodeCount(), 0);
        for (std::size_t i = 0; i < classes.size() && i < depth.size(); ++i)
        {
            if (!isNoData(depth[i], fillValue))
            {
                classes[i] = static_cast<double>(depth[i]) < threshold ? 1 : 2;
            }
        }
        return collectEdges(grid, classes, edgeTable({ { 1, 2 } }));
    }

    GridEdges boundaryEdgesOfClasses(const RegularGrid& grid, std::span<const std::uint8_t> classes)
    {
        return collectEdges(grid, classes, edgeTable({ { BoundaryInside, BoundaryOutside } }));
    }

    GridEdges boundaryEdges(const RegularGrid& grid, const std::function<bool(std::size_t)>& inside, const std::function<bool(std::size_t)>& draw)
    {
        std::vector<std::uint8_t> classes(grid.nodeCount(), BoundaryOutside);
        for (std::size_t i = 0; i < classes.size(); ++i)
        {
            if (inside(i))
            {
                classes[i] = !draw || draw(i) ? BoundaryInside : BoundaryInsideUndrawn;
            }
        }
        return boundaryEdgesOfClasses(grid, classes);
    }

    DepthLines depthLines(const DepthLinesInput& input)
    {
        DepthLines lines;
        if (!input.grid)
        {
            return lines;
        }
        const RegularGrid& grid = *input.grid;
        if (input.esc)
        {
            lines.safetyContour = safetyContourEdges(grid, input.depth, input.fillValue, input.safetyContour);
            // The S-102 boundary is that of the cells with data; where it coincides with the WLA
            // boundary only the WLA boundary is drawn (D-2.4).
            std::vector<std::uint8_t> classes(input.depth.size(), BoundaryOutside);
            for (std::size_t i = 0; i < classes.size(); ++i)
            {
                if (isFill(input.depth[i], input.fillValue))
                {
                    continue;
                }
                const bool adjusted = input.wlaActive && i < input.applied.size() && input.applied[i] == 1;
                classes[i] = adjusted ? BoundaryInsideUndrawn : BoundaryInside;
            }
            lines.s102Boundary = boundaryEdgesOfClasses(grid, classes);
        }
        if (input.wlaActive && input.appliedCount > 0 && input.applied.size() >= grid.nodeCount())
        {
            lines.wlaBoundary = boundaryEdgesOfClasses(grid, input.applied);
        }
        return lines;
    }

    std::vector<GridRing> validGridRings(const RegularGrid& grid, std::span<const float> values, double fillValue)
    {
        const int cols = grid.cols;
        const int rows = grid.rows;
        const std::size_t cells = grid.nodeCount();
        std::vector<std::uint8_t> valid(cells, 0);
        for (std::size_t i = 0; i < cells && i < values.size(); ++i)
        {
            if (!isNoData(values[i], fillValue)) valid[i] = 1;
        }
        // Vertices are the cells' corners, (cols + 1) x (rows + 1); each directed edge leaving a
        // vertex is a bit per direction: +col, +row, -col, -row.
        const int stride = cols + 1;
        std::vector<std::uint8_t> leaving(static_cast<std::size_t>(stride) * static_cast<std::size_t>(rows + 1), 0);
        const auto at = [&](int row, int col) { return static_cast<std::size_t>(row) * static_cast<std::size_t>(stride) + static_cast<std::size_t>(col); };
        const auto isValid = [&](int row, int col) { return valid[static_cast<std::size_t>(row) * static_cast<std::size_t>(cols) + static_cast<std::size_t>(col)] != 0; };
        // Each valid cell contributes the sides that face an empty cell, walked clockwise in index
        // space, so shared sides are never emitted and every ring closes.
        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                if (!isValid(row, col)) continue;
                if (row == 0 || !isValid(row - 1, col)) leaving[at(row, col)] |= 1;
                if (col == cols - 1 || !isValid(row, col + 1)) leaving[at(row, col + 1)] |= 2;
                if (row == rows - 1 || !isValid(row + 1, col)) leaving[at(row + 1, col + 1)] |= 4;
                if (col == 0 || !isValid(row, col - 1)) leaving[at(row + 1, col)] |= 8;
            }
        }

        static constexpr int stepCol[4] = { 1, 0, -1, 0 };
        static constexpr int stepRow[4] = { 0, 1, 0, -1 };
        // Takes an edge leaving `vertex`, preferring the right turn from `arriving` (two regions
        // touching at a corner stay two rings), and clears it.
        const auto take = [&](std::size_t vertex, int arriving) -> int {
            const unsigned bits = leaving[vertex];
            if (bits == 0) return -1;
            const int preferred = (arriving + 1) % 4;
            const int direction = (bits & (1u << preferred)) ? preferred : std::countr_zero(bits);
            leaving[vertex] = static_cast<std::uint8_t>(bits & ~(1u << direction));
            return direction;
        };

        std::vector<GridRing> rings;
        for (std::size_t start = 0; start < leaving.size(); ++start)
        {
            while (leaving[start] != 0)
            {
                const int startCol = static_cast<int>(start % static_cast<std::size_t>(stride));
                const int startRow = static_cast<int>(start / static_cast<std::size_t>(stride));
                std::vector<int> corners{ startCol, startRow };
                int col = startCol;
                int row = startRow;
                int direction = take(start, 3);
                for (;;)
                {
                    // Run straight while the next edge keeps the direction: one vertex per turn.
                    col += stepCol[direction];
                    row += stepRow[direction];
                    if (col == startCol && row == startRow) break;
                    const int next = take(at(row, col), direction);
                    if (next < 0) break;
                    if (next != direction) corners.insert(corners.end(), { col, row });
                    direction = next;
                }
                // Corners are cell corners, at node index -0.5; a projected grid's sides are bent.
                GridRing ring;
                const std::size_t count = corners.size() / 2;
                for (std::size_t i = 0; i < count; ++i)
                {
                    const int c0 = corners[2 * i];
                    const int r0 = corners[2 * i + 1];
                    if (grid.utm)
                    {
                        const int c1 = corners[(2 * i + 2) % corners.size()];
                        const int r1 = corners[(2 * i + 3) % corners.size()];
                        const auto line = gridLine(grid, c0 - 0.5, r0 - 0.5, c1 - 0.5, r1 - 0.5);
                        ring.exterior.insert(ring.exterior.end(), line.begin(), line.end() - 1);
                    }
                    else
                    {
                        ring.exterior.push_back({ grid.originLon + (c0 - 0.5) * grid.spacingLon, grid.originLat + (r0 - 0.5) * grid.spacingLat });
                    }
                }
                if (ring.exterior.size() >= 3)
                {
                    rings.push_back(std::move(ring));
                }
            }
        }
        return rings;
    }

    Rgb xyLToRgb(double x, double y, double L, double whiteL)
    {
        const double Y = L / whiteL;
        const double X = (x / y) * Y;
        const double Z = ((1 - x - y) / y) * Y;
        std::array<double, 3> linear{
            3.2406 * X - 1.5372 * Y - 0.4986 * Z,
            -0.9689 * X + 1.8758 * Y + 0.0415 * Z,
            0.0557 * X - 0.204 * Y + 1.057 * Z,
        };
        for (double& v : linear) v = std::max(0.0, v);
        const double peak = std::max({ 1.0, linear[0], linear[1], linear[2] });
        const auto encode = [peak](double v) {
            const double c = v / peak;
            const double encoded = c <= 0.0031308 ? 12.92 * c : 1.055 * std::pow(c, 1 / 2.4) - 0.055;
            return static_cast<std::uint8_t>(std::clamp(std::lround(255 * encoded), 0L, 255L));
        };
        return Rgb{ encode(linear[0]), encode(linear[1]), encode(linear[2]) };
    }

    Rgb depwlColor(std::string_view palette)
    {
        const double L = palette == "Dusk" ? 7.5 : palette == "Night" ? 1.2 : 30;
        return xyLToRgb(0.15, 0.15, L);
    }

    const S98LineStyle& safetyContourStyle()
    {
        static const S98LineStyle style{ "DEPSC", 0.64, {} };
        return style;
    }

    const S98LineStyle& s102BoundaryStyle()
    {
        static const S98LineStyle style{ "DEPWL", 0.96, { 1.92, 1.92 } };
        return style;
    }

    const S98LineStyle& wlaBoundaryStyle()
    {
        static const S98LineStyle style{ "DEPWL", 1.92, { 7.68, 5.76, 1.92, 5.76 } };
        return style;
    }
}
