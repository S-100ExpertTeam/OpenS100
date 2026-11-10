#pragma once

// Geographic positions and extents of the coverage module, in degrees (WGS 84).
namespace s100::coverage
{
    struct GeoPosition
    {
        double lon = 0;
        double lat = 0;

        friend bool operator==(const GeoPosition&, const GeoPosition&) = default;
    };

    struct GeoBounds
    {
        double minLon = 0;
        double minLat = 0;
        double maxLon = 0;
        double maxLat = 0;

        [[nodiscard]] bool intersects(const GeoBounds& other) const noexcept
        {
            return minLon <= other.maxLon && maxLon >= other.minLon && minLat <= other.maxLat && maxLat >= other.minLat;
        }

        friend bool operator==(const GeoBounds&, const GeoBounds&) = default;
    };
}
