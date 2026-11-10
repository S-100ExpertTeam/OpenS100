#include "RegularGrid.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numbers>

namespace s100::coverage
{
    namespace
    {
        GeoBounds boundsOf(const std::vector<GeoPosition>& positions) noexcept
        {
            constexpr double inf = std::numeric_limits<double>::infinity();
            GeoBounds box{ inf, inf, -inf, -inf };
            for (const auto& [lon, lat] : positions)
            {
                box.minLon = std::min(box.minLon, lon);
                box.maxLon = std::max(box.maxLon, lon);
                box.minLat = std::min(box.minLat, lat);
                box.maxLat = std::max(box.maxLat, lat);
            }
            return box;
        }

        // JavaScript Math.round: halves go up, also below zero.
        int roundHalfUp(double value) noexcept
        {
            return static_cast<int>(std::floor(value + 0.5));
        }

        // Most lattice points between converted ones, and the most ground between them, in metres.
        constexpr int MeshMaxStep = 32;
        constexpr double MeshMaxMetres = 1000;
        constexpr double MetresPerDegree = 111320;
    }

    GeoPosition gridPosition(const RegularGrid& grid, double col, double row) noexcept
    {
        const double x = grid.originLon + col * grid.spacingLon;
        const double y = grid.originLat + row * grid.spacingLat;
        return grid.utm ? utmToGeographic(*grid.utm, x, y) : GeoPosition{ x, y };
    }

    GridIndex gridCoordinates(const RegularGrid& grid, const GeoPosition& position) noexcept
    {
        double x = position.lon;
        double y = position.lat;
        if (grid.utm)
        {
            const UtmPoint point = geographicToUtm(*grid.utm, position);
            x = point.easting;
            y = point.northing;
        }
        return GridIndex{ (x - grid.originLon) / grid.spacingLon, (y - grid.originLat) / grid.spacingLat };
    }

    std::vector<GeoPosition> gridLine(const RegularGrid& grid, double col0, double row0, double col1, double row1)
    {
        const double cells = std::max(std::abs(col1 - col0), std::abs(row1 - row0));
        const int pieces = grid.utm ? std::max(1, static_cast<int>(std::ceil(cells / ProjectedLineCells))) : 1;
        std::vector<GeoPosition> line;
        line.reserve(static_cast<std::size_t>(pieces) + 1);
        for (int i = 0; i <= pieces; ++i)
        {
            line.push_back(gridPosition(grid, col0 + ((col1 - col0) * i) / pieces, row0 + ((row1 - row0) * i) / pieces));
        }
        return line;
    }

    std::vector<GeoPosition> gridBlockRing(const RegularGrid& grid, double col0, double row0, double col1, double row1)
    {
        std::vector<GeoPosition> ring;
        const auto add = [&](std::vector<GeoPosition> line) { ring.insert(ring.end(), line.begin(), line.end() - 1); };
        add(gridLine(grid, col0, row0, col1, row0));
        add(gridLine(grid, col1, row0, col1, row1));
        add(gridLine(grid, col1, row1, col0, row1));
        add(gridLine(grid, col0, row1, col0, row0));
        return ring;
    }

    GeoBounds gridBounds(const RegularGrid& grid)
    {
        return boundsOf(gridBlockRing(grid, 0, 0, grid.cols - 1, grid.rows - 1));
    }

    GeoBounds gridCellBounds(const RegularGrid& grid)
    {
        return boundsOf(gridBlockRing(grid, -0.5, -0.5, grid.cols - 0.5, grid.rows - 0.5));
    }

    CellSize gridCellSize(const RegularGrid& grid) noexcept
    {
        if (!grid.utm)
        {
            return CellSize{ std::abs(grid.spacingLon), std::abs(grid.spacingLat) };
        }
        const double col = (grid.cols - 1) / 2.0;
        const double row = (grid.rows - 1) / 2.0;
        return CellSize{
            std::abs(gridPosition(grid, col + 0.5, row).lon - gridPosition(grid, col - 0.5, row).lon),
            std::abs(gridPosition(grid, col, row + 0.5).lat - gridPosition(grid, col, row - 0.5).lat),
        };
    }

    std::optional<GridCell> gridCellAt(const RegularGrid& grid, const GeoPosition& position) noexcept
    {
        const GridIndex at = gridCoordinates(grid, position);
        if (!std::isfinite(at.col) || !std::isfinite(at.row))
        {
            return std::nullopt;
        }
        const int col = roundHalfUp(at.col);
        const int row = roundHalfUp(at.row);
        if (!(col >= 0 && col < grid.cols && row >= 0 && row < grid.rows))
        {
            return std::nullopt;
        }
        return GridCell{ col, row };
    }

    std::optional<float> sampleGrid(const RegularGrid& grid, std::span<const float> values, double fillValue,
        const GeoPosition& position) noexcept
    {
        const auto cell = gridCellAt(grid, position);
        if (!cell)
        {
            return std::nullopt;
        }
        const std::size_t index = static_cast<std::size_t>(cell->row) * static_cast<std::size_t>(grid.cols) + static_cast<std::size_t>(cell->col);
        const float value = index < values.size() ? values[index] : std::numeric_limits<float>::quiet_NaN();
        if (isNoData(value, fillValue))
        {
            return std::nullopt;
        }
        return value;
    }

    std::optional<ValueRange> valueRange(std::span<const float> values, double fillValue) noexcept
    {
        double min = std::numeric_limits<double>::infinity();
        double max = -std::numeric_limits<double>::infinity();
        for (const float value : values)
        {
            if (isNoData(value, fillValue))
            {
                continue;
            }
            min = std::min(min, static_cast<double>(value));
            max = std::max(max, static_cast<double>(value));
        }
        if (!(min <= max))
        {
            return std::nullopt;
        }
        return ValueRange{ min, max };
    }

    LatticeMapping::LatticeMapping(int cols, int rows, const PositionAt& positionAt, const RegularGrid& target)
        : cols_(cols), rows_(rows)
    {
        const GeoPosition a = positionAt(0, 0);
        const GeoPosition b = positionAt(1, 1);
        const double spacing = MetresPerDegree
            * std::min(std::abs(b.lon - a.lon) * std::cos(a.lat * std::numbers::pi / 180), std::abs(b.lat - a.lat));
        step_ = std::min(MeshMaxStep, std::max(1, static_cast<int>(std::floor(MeshMaxMetres / std::max(spacing, 1e-9)))));
        meshCols_ = static_cast<int>(std::ceil((cols - 1) / static_cast<double>(step_))) + 1;
        meshRows_ = static_cast<int>(std::ceil((rows - 1) / static_cast<double>(step_))) + 1;
        meshCol_.resize(static_cast<std::size_t>(meshCols_) * static_cast<std::size_t>(meshRows_));
        meshRow_.resize(meshCol_.size());
        lineCol_.resize(static_cast<std::size_t>(meshCols_));
        lineRow_.resize(static_cast<std::size_t>(meshCols_));
        for (int my = 0; my < meshRows_; ++my)
        {
            for (int mx = 0; mx < meshCols_; ++mx)
            {
                const GridIndex at = gridCoordinates(target, positionAt(mx * step_, my * step_));
                const std::size_t index = static_cast<std::size_t>(my) * static_cast<std::size_t>(meshCols_) + static_cast<std::size_t>(mx);
                meshCol_[index] = at.col;
                meshRow_[index] = at.row;
            }
        }
    }

    void LatticeMapping::line(double y, std::span<double> outCol, std::span<double> outRow)
    {
        const int my = std::min(static_cast<int>(std::floor(y / step_)), std::max(0, meshRows_ - 2));
        const double fy = meshRows_ > 1 ? y / step_ - my : 0;
        const int above = std::min(my + 1, meshRows_ - 1);
        for (int mx = 0; mx < meshCols_; ++mx)
        {
            const std::size_t a = static_cast<std::size_t>(my) * static_cast<std::size_t>(meshCols_) + static_cast<std::size_t>(mx);
            const std::size_t b = static_cast<std::size_t>(above) * static_cast<std::size_t>(meshCols_) + static_cast<std::size_t>(mx);
            lineCol_[static_cast<std::size_t>(mx)] = meshCol_[a] + (meshCol_[b] - meshCol_[a]) * fy;
            lineRow_[static_cast<std::size_t>(mx)] = meshRow_[a] + (meshRow_[b] - meshRow_[a]) * fy;
        }
        const auto cols = static_cast<std::size_t>(cols_);
        if (meshCols_ == 1)
        {
            std::fill_n(outCol.begin(), cols, lineCol_[0]);
            std::fill_n(outRow.begin(), cols, lineRow_[0]);
            return;
        }
        // Along the line, each mesh span is walked by its per-point increment: the millions of
        // points of a screen-sized lattice cannot each afford a division and a floor.
        std::size_t x = 0;
        for (int mx = 0; mx < meshCols_ - 1 && x < cols; ++mx)
        {
            const auto i = static_cast<std::size_t>(mx);
            const double dc = (lineCol_[i + 1] - lineCol_[i]) / step_;
            const double dr = (lineRow_[i + 1] - lineRow_[i]) / step_;
            double col = lineCol_[i];
            double row = lineRow_[i];
            const std::size_t end = std::min(cols, static_cast<std::size_t>(mx + 1) * static_cast<std::size_t>(step_));
            for (; x < end; ++x)
            {
                outCol[x] = col;
                outRow[x] = row;
                col += dc;
                row += dr;
            }
            // The last span carries on past its end for the points beyond the last mesh column.
            if (mx == meshCols_ - 2)
            {
                for (; x < cols; ++x)
                {
                    outCol[x] = col;
                    outRow[x] = row;
                    col += dc;
                    row += dr;
                }
            }
        }
    }
}
