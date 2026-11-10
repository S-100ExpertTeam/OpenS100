#pragma once

#include "Geo.h"
#include "Utm.h"

#include <cmath>
#include <functional>
#include <optional>
#include <span>
#include <vector>

// A regular grid (dataCodingFormat 2) of an S-100 Part 10c coverage, and the geometry of its
// nodes and cells (opens100-web `core/src/coverage.ts`).
namespace s100::coverage
{
    // Values are stored row-major with row 0 at originLat (the south edge in every product seen
    // so far); each value belongs to its node, the centre of its cell. A file whose data points
    // are offset from its grid points has its origin moved by the reader, so the nodes here are
    // the data points either way. Origin and spacing are degrees of longitude and latitude, or,
    // with `utm` set, eastings and northings in metres. Go between node indices and geographic
    // positions with gridPosition and gridCoordinates rather than with the numbers here.
    struct RegularGrid
    {
        double originLon = 0;
        double originLat = 0;
        double spacingLon = 0;
        double spacingLat = 0;
        int cols = 0;
        int rows = 0;
        std::optional<UtmZone> utm;   // the WGS 84 / UTM zone the grid is laid out on

        [[nodiscard]] std::size_t nodeCount() const noexcept
        {
            return static_cast<std::size_t>(cols > 0 ? cols : 0) * static_cast<std::size_t>(rows > 0 ? rows : 0);
        }

        friend bool operator==(const RegularGrid&, const RegularGrid&) = default;
    };

    // Fractional node indices; the edge between columns k-1 and k is at col = k - 0.5.
    struct GridIndex
    {
        double col = 0;
        double row = 0;
    };

    struct GridCell
    {
        int col = 0;
        int row = 0;

        friend bool operator==(const GridCell&, const GridCell&) = default;
    };

    struct CellSize
    {
        double lon = 0;   // degrees
        double lat = 0;
    };

    // Geographic position of node (col, row); fractional indices are fine.
    [[nodiscard]] GeoPosition gridPosition(const RegularGrid& grid, double col, double row) noexcept;
    // The fractional node indices of `position`; the inverse of gridPosition.
    [[nodiscard]] GridIndex gridCoordinates(const RegularGrid& grid, const GeoPosition& position) noexcept;

    // How many cells a straight piece of a projected grid line may span: grid lines of a UTM grid
    // are curves on a Mercator chart, so they are handed out in pieces short enough to be drawn
    // straight within millimetres.
    inline constexpr int ProjectedLineCells = 32;

    // The grid line from (col0, row0) to (col1, row1), both ends included, as positions close
    // enough together to be joined by straight lines (two for a geographic grid).
    [[nodiscard]] std::vector<GeoPosition> gridLine(const RegularGrid& grid, double col0, double row0, double col1, double row1);
    // The outline of the block of cells from edge col0 to col1 and row0 to row1, unclosed.
    [[nodiscard]] std::vector<GeoPosition> gridBlockRing(const RegularGrid& grid, double col0, double row0, double col1, double row1);
    // Geographic extent of the grid nodes.
    [[nodiscard]] GeoBounds gridBounds(const RegularGrid& grid);
    // Geographic extent of the grid's cells: half a cell beyond the outermost nodes.
    [[nodiscard]] GeoBounds gridCellBounds(const RegularGrid& grid);
    // Width and height of the cell in the middle of the grid, in degrees.
    [[nodiscard]] CellSize gridCellSize(const RegularGrid& grid) noexcept;
    // The grid node nearest to `position`, or std::nullopt when it falls outside the grid's cells.
    [[nodiscard]] std::optional<GridCell> gridCellAt(const RegularGrid& grid, const GeoPosition& position) noexcept;

    // Whether a value is "no data": not finite, or the fill value. Values are float32 and are
    // compared widened to double, as opens100-web compares a Float32Array value with its fill.
    [[nodiscard]] inline bool isNoData(float value, double fillValue) noexcept
    {
        return !std::isfinite(value) || static_cast<double>(value) == fillValue;
    }

    // Nearest-node value at `position`, or std::nullopt outside the grid or on a fill value.
    [[nodiscard]] std::optional<float> sampleGrid(const RegularGrid& grid, std::span<const float> values, double fillValue,
        const GeoPosition& position) noexcept;

    struct ValueRange
    {
        double min = 0;
        double max = 0;

        friend bool operator==(const ValueRange&, const ValueRange&) = default;
    };

    // Minimum and maximum over the values that are not fill values; std::nullopt when none.
    [[nodiscard]] std::optional<ValueRange> valueRange(std::span<const float> values, double fillValue) noexcept;

    // Where the points of a regular lattice fall in `target`, as fractional node indices.
    // Converting every point of a lattice as fine as an S-102 grid is slow on a projected CRS, so
    // only points up to a kilometre apart are converted and the rest interpolated; UTM, geographic
    // and Mercator coordinates bend against each other by centimetres over that distance.
    class LatticeMapping final
    {
    public:
        using PositionAt = std::function<GeoPosition(double x, double y)>;

        // `positionAt(x, y)` is the geographic position of lattice point x (0..cols-1), y (0..rows-1).
        LatticeMapping(int cols, int rows, const PositionAt& positionAt, const RegularGrid& target);

        // Target indices of lattice points (0..cols-1, y), written to `outCol` and `outRow`
        // (each at least cols long).
        void line(double y, std::span<double> outCol, std::span<double> outRow);

        [[nodiscard]] int cols() const noexcept { return cols_; }
        [[nodiscard]] int rows() const noexcept { return rows_; }

    private:
        int cols_;
        int rows_;
        int step_ = 1;
        int meshCols_ = 1;
        int meshRows_ = 1;
        std::vector<double> meshCol_;
        std::vector<double> meshRow_;
        std::vector<double> lineCol_;
        std::vector<double> lineRow_;
    };
}
