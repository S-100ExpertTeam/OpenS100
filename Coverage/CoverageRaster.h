#pragma once

#include "CoverageLookup.h"
#include "RegularGrid.h"

#include <cstdint>
#include <functional>
#include <optional>
#include <span>
#include <string_view>
#include <vector>

// A coverage grid coloured once into an image laid out as the chart is drawn, ready to be drawn in
// one call (opens100-web `packages/render/src/coverage-renderer.ts`). Independent of any drawing
// back end.
namespace s100::coverage
{
    // Latitude in degrees to a Web Mercator northing expressed in degrees, and back.
    [[nodiscard]] double mercatorY(double lat) noexcept;
    [[nodiscard]] double inverseMercatorY(double y) noexcept;

    // How the chart the image goes onto spaces its rows: evenly in Web Mercator northing (opens100-web,
    // OpenLayers) or evenly in latitude (an equirectangular chart, such as the OpenS100 viewer's).
    // The image's rows must be spaced as the chart's are, or the rows between its corners drift
    // from where the grid's cells are drawn by anything else.
    enum class RasterProjection
    {
        WebMercator,
        Geographic,
    };

    // A rectangle on the chart: degrees of longitude, and northings in the raster's projection
    // (mercatorY for Web Mercator, degrees of latitude for Geographic).
    struct MercatorExtent
    {
        double west = 0;
        double east = 0;
        double south = 0;
        double north = 0;
    };

    struct Rgba
    {
        std::uint8_t r = 0;
        std::uint8_t g = 0;
        std::uint8_t b = 0;
        std::uint8_t a = 0;

        friend bool operator==(const Rgba&, const Rgba&) = default;
    };

    struct Rgb
    {
        std::uint8_t r = 0;
        std::uint8_t g = 0;
        std::uint8_t b = 0;
    };

    // The colour of a value, or std::nullopt to leave its cell empty.
    using CellColor = std::function<std::optional<Rgba>(double value)>;
    // A palette's sRGB colour of a colour token.
    using ColorResolver = std::function<Rgb(std::string_view token)>;

    // Colours values by a CoverageFill lookup table: the first matching entry's colour (with its
    // transparency as alpha, lerped across the range for a gradient); no match leaves the cell empty.
    [[nodiscard]] CellColor lookupCellColor(std::span<const LookupEntry> lookup, const ColorResolver& resolve);

    class CoverageRaster final
    {
    public:
        // Rows a raster may have: past this a grid is finer than any screen.
        static constexpr int MaxRows = 16384;

        // A geographic grid gives an image as wide as the grid, its rows resampled to the
        // projection (north first; on Geographic, one image row per grid row); a grid on UTM is
        // warped into the projection at about its cell size.
        CoverageRaster(const RegularGrid& grid, std::span<const float> values, double fillValue, const CellColor& colorOf,
            RasterProjection projection = RasterProjection::WebMercator);

        [[nodiscard]] int width() const noexcept { return width_; }
        [[nodiscard]] int height() const noexcept { return height_; }
        // RGBA per pixel (bytes r, g, b, a), row 0 at the north; 0 where nothing is drawn.
        [[nodiscard]] std::span<const std::uint32_t> pixels() const noexcept { return pixels_; }
        [[nodiscard]] const MercatorExtent& extent() const noexcept { return extent_; }
        [[nodiscard]] const RegularGrid& grid() const noexcept { return grid_; }
        [[nodiscard]] RasterProjection projection() const noexcept { return projection_; }
        // The latitude of a northing of the extent.
        [[nodiscard]] double latitude(double northing) const noexcept;

    private:
        RegularGrid grid_;
        RasterProjection projection_;
        int width_ = 0;
        int height_ = 0;
        std::vector<std::uint32_t> pixels_;
        MercatorExtent extent_;
    };
}
