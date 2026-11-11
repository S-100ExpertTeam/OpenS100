#include "CoverageRaster.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numbers>

namespace s100::coverage
{
    namespace
    {
        constexpr double MaxMercatorLat = 85.0511287798066;
        constexpr double Degree = std::numbers::pi / 180;

        std::uint32_t pack(const Rgba& color) noexcept
        {
            return static_cast<std::uint32_t>(color.r) | (static_cast<std::uint32_t>(color.g) << 8)
                | (static_cast<std::uint32_t>(color.b) << 16) | (static_cast<std::uint32_t>(color.a) << 24);
        }

        std::uint8_t channel(double value) noexcept
        {
            return static_cast<std::uint8_t>(std::clamp(std::lround(value), 0L, 255L));
        }
    }

    double mercatorY(double lat) noexcept
    {
        const double clamped = std::clamp(lat, -MaxMercatorLat, MaxMercatorLat);
        return (180 / std::numbers::pi) * std::log(std::tan(std::numbers::pi / 4 + clamped * Degree / 2));
    }

    double inverseMercatorY(double y) noexcept
    {
        return (180 / std::numbers::pi) * (2 * std::atan(std::exp(y * Degree)) - std::numbers::pi / 2);
    }

    CellColor lookupCellColor(std::span<const LookupEntry> lookup, const ColorResolver& resolve)
    {
        struct Colors
        {
            Rgba start;
            std::optional<Rgba> end;
        };
        const auto rgba = [&](std::string_view token, double transparency) {
            const Rgb color = resolve(token);
            return Rgba{ color.r, color.g, color.b, channel(255 * (1 - std::clamp(transparency, 0.0, 1.0))) };
        };
        std::vector<std::optional<Colors>> colors;
        for (const LookupEntry& entry : lookup)
        {
            if (!entry.color)
            {
                colors.emplace_back();
                continue;
            }
            Colors item{ rgba(entry.color->startToken, entry.color->startTransparency), std::nullopt };
            if (entry.color->endToken)
            {
                item.end = rgba(*entry.color->endToken, entry.color->endTransparency);
            }
            colors.emplace_back(item);
        }
        std::vector<LookupEntry> entries(lookup.begin(), lookup.end());
        return [entries = std::move(entries), colors = std::move(colors)](double value) -> std::optional<Rgba> {
            const auto match = lookupCoverageEntry(entries, value);
            if (!match || !colors[match->index])
            {
                return std::nullopt;
            }
            const Colors& color = *colors[match->index];
            if (!color.end)
            {
                return color.start;
            }
            const double f = match->fraction;
            const auto lerp = [f](std::uint8_t a, std::uint8_t b) { return channel(a + (static_cast<double>(b) - a) * f); };
            return Rgba{ lerp(color.start.r, color.end->r), lerp(color.start.g, color.end->g), lerp(color.start.b, color.end->b),
                lerp(color.start.a, color.end->a) };
        };
    }

    double CoverageRaster::latitude(double northing) const noexcept
    {
        return projection_ == RasterProjection::Geographic ? northing : inverseMercatorY(northing);
    }

    CoverageRaster::CoverageRaster(const RegularGrid& grid, std::span<const float> values, double fillValue, const CellColor& colorOf,
        RasterProjection projection)
        : grid_(grid), projection_(projection)
    {
        // Latitude to the chart's northing, and back.
        const bool geographic = projection == RasterProjection::Geographic;
        const auto northing = [geographic](double lat) { return geographic ? lat : mercatorY(lat); };
        const auto latitudeOf = [geographic](double y) { return geographic ? y : inverseMercatorY(y); };

        const std::size_t count = grid.nodeCount();
        std::vector<std::uint32_t> cells(count, 0);
        for (std::size_t i = 0; i < count && i < values.size(); ++i)
        {
            const float value = values[i];
            if (isNoData(value, fillValue))
            {
                continue;
            }
            if (const auto color = colorOf(value))
            {
                cells[i] = pack(*color);
            }
        }

        if (!grid.utm)
        {
            // A geographic grid is a rectangle on the chart too, its columns evenly spaced and its
            // rows stretched with latitude in Mercator (not at all on Geographic): resampled row by
            // row, it goes onto the chart in one call.
            const auto latEdge = [&](double edge) { return grid.originLat + (edge - 0.5) * grid.spacingLat; };
            const auto lonEdge = [&](double edge) { return grid.originLon + (edge - 0.5) * grid.spacingLon; };
            const double north = northing(std::max(latEdge(0), latEdge(grid.rows)));
            const double south = northing(std::min(latEdge(0), latEdge(grid.rows)));
            extent_ = MercatorExtent{ std::min(lonEdge(0), lonEdge(grid.cols)), std::max(lonEdge(0), lonEdge(grid.cols)), south, north };
            // Enough rows that the grid row shortest in Mercator still gets one.
            const double shortest = std::min(std::abs(northing(latEdge(1)) - northing(latEdge(0))),
                std::abs(northing(latEdge(grid.rows)) - northing(latEdge(grid.rows - 1))));
            height_ = std::min(MaxRows, std::max(grid.rows,
                static_cast<int>(std::lround((north - south) / std::max(shortest, std::numeric_limits<double>::epsilon())))));
            width_ = grid.cols;
            const double pixelY = (north - south) / height_;
            pixels_.assign(static_cast<std::size_t>(width_) * static_cast<std::size_t>(height_), 0);
            for (int y = 0; y < height_; ++y)
            {
                const double lat = latitudeOf(north - (y + 0.5) * pixelY);
                const int row = std::min(grid.rows - 1, std::max(0, static_cast<int>(std::floor((lat - latEdge(0)) / grid.spacingLat))));
                std::copy_n(cells.begin() + static_cast<std::ptrdiff_t>(row) * grid.cols, grid.cols,
                    pixels_.begin() + static_cast<std::ptrdiff_t>(y) * width_);
            }
            return;
        }

        // Pixels as fine as the grid's cells, each showing the cell its centre falls in.
        const GeoBounds bounds = gridCellBounds(grid);
        const CellSize size = gridCellSize(grid);
        const double middle = (bounds.minLat + bounds.maxLat) / 2;
        extent_ = MercatorExtent{ bounds.minLon, bounds.maxLon, northing(bounds.minLat), northing(bounds.maxLat) };
        width_ = std::max(1, static_cast<int>(std::ceil((extent_.east - extent_.west) / size.lon)));
        height_ = std::max(1, static_cast<int>(std::ceil((extent_.north - extent_.south)
            / (northing(middle + size.lat / 2) - northing(middle - size.lat / 2)))));
        const double pixelLon = (extent_.east - extent_.west) / width_;
        const double pixelY = (extent_.north - extent_.south) / height_;
        LatticeMapping mapping(width_, height_, [&](double x, double y) {
            return GeoPosition{ extent_.west + (x + 0.5) * pixelLon, latitudeOf(extent_.north - (y + 0.5) * pixelY) };
        }, grid);
        pixels_.assign(static_cast<std::size_t>(width_) * static_cast<std::size_t>(height_), 0);
        std::vector<double> cols(static_cast<std::size_t>(width_));
        std::vector<double> rows(static_cast<std::size_t>(width_));
        for (int y = 0; y < height_; ++y)
        {
            mapping.line(y, cols, rows);
            for (int x = 0; x < width_; ++x)
            {
                const int col = static_cast<int>(std::floor(cols[static_cast<std::size_t>(x)] + 0.5));
                const int row = static_cast<int>(std::floor(rows[static_cast<std::size_t>(x)] + 0.5));
                if (col >= 0 && col < grid.cols && row >= 0 && row < grid.rows)
                {
                    pixels_[static_cast<std::size_t>(y) * static_cast<std::size_t>(width_) + static_cast<std::size_t>(x)]
                        = cells[static_cast<std::size_t>(row) * static_cast<std::size_t>(grid.cols) + static_cast<std::size_t>(col)];
                }
            }
        }
    }
}
