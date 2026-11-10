#pragma once

#include "Geo.h"

#include <optional>

// WGS 84 / UTM, the projected horizontal CRS S-102 allows besides EPSG:4326 (EPSG:32601-32660
// and 32701-32760) and the one NOAA's datasets use. A grid on it keeps its eastings and
// northings (RegularGrid::utm); these conversions relate it to geographic positions. The Krueger
// series to n^3, as in opens100-web `core/src/utm.ts`: sub-millimetre within a zone.
namespace s100::coverage
{
    struct UtmZone
    {
        int zone = 0;   // 1-60
        bool south = false;

        friend bool operator==(const UtmZone&, const UtmZone&) = default;
    };

    struct UtmPoint
    {
        double easting = 0;
        double northing = 0;
    };

    // The WGS 84 / UTM zone of an EPSG code, or std::nullopt for any other CRS.
    [[nodiscard]] std::optional<UtmZone> utmZoneFromEpsg(int code) noexcept;

    [[nodiscard]] UtmPoint geographicToUtm(const UtmZone& utm, const GeoPosition& position) noexcept;
    [[nodiscard]] GeoPosition utmToGeographic(const UtmZone& utm, double easting, double northing) noexcept;
}
