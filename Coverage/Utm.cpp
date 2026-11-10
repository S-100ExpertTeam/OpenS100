#include "Utm.h"

#include <array>
#include <cmath>
#include <numbers>

namespace s100::coverage
{
    namespace
    {
        constexpr double A = 6378137;
        constexpr double F = 1 / 298.257223563;
        constexpr double K0 = 0.9996;
        constexpr double FalseEasting = 500000;
        constexpr double FalseNorthingSouth = 10000000;
        constexpr double N = F / (2 - F);
        constexpr double N2 = N * N;
        constexpr double N3 = N2 * N;
        constexpr double Rectifying = (A / (1 + N)) * (1 + N2 / 4 + N2 * N2 / 64);
        constexpr std::array<double, 3> Alpha{ N / 2 - (2 * N2) / 3 + (5 * N3) / 16, (13 * N2) / 48 - (3 * N3) / 5, (61 * N3) / 240 };
        constexpr std::array<double, 3> Beta{ N / 2 - (2 * N2) / 3 + (37 * N3) / 96, N2 / 48 + N3 / 15, (17 * N3) / 480 };
        constexpr std::array<double, 3> Delta{ 2 * N - (2 * N2) / 3 - 2 * N3, (7 * N2) / 3 - (8 * N3) / 5, (56 * N3) / 15 };
        constexpr double Rad = std::numbers::pi / 180;

        double centralMeridian(const UtmZone& utm) noexcept
        {
            return (utm.zone * 6 - 183) * Rad;
        }
    }

    std::optional<UtmZone> utmZoneFromEpsg(int code) noexcept
    {
        if (code >= 32601 && code <= 32660) return UtmZone{ code - 32600, false };
        if (code >= 32701 && code <= 32760) return UtmZone{ code - 32700, true };
        return std::nullopt;
    }

    UtmPoint geographicToUtm(const UtmZone& utm, const GeoPosition& position) noexcept
    {
        const double phi = position.lat * Rad;
        const double lambda = position.lon * Rad - centralMeridian(utm);
        const double c = (2 * std::sqrt(N)) / (1 + N);
        const double t = std::sinh(std::atanh(std::sin(phi)) - c * std::atanh(c * std::sin(phi)));
        const double xi = std::atan2(t, std::cos(lambda));
        const double eta = std::atanh(std::sin(lambda) / std::sqrt(1 + t * t));
        double x = eta;
        double y = xi;
        for (int j = 1; j <= 3; ++j)
        {
            x += Alpha[j - 1] * std::cos(2 * j * xi) * std::sinh(2 * j * eta);
            y += Alpha[j - 1] * std::sin(2 * j * xi) * std::cosh(2 * j * eta);
        }
        return UtmPoint{ FalseEasting + K0 * Rectifying * x, (utm.south ? FalseNorthingSouth : 0) + K0 * Rectifying * y };
    }

    GeoPosition utmToGeographic(const UtmZone& utm, double easting, double northing) noexcept
    {
        const double xi = (northing - (utm.south ? FalseNorthingSouth : 0)) / (K0 * Rectifying);
        const double eta = (easting - FalseEasting) / (K0 * Rectifying);
        double xi1 = xi;
        double eta1 = eta;
        for (int j = 1; j <= 3; ++j)
        {
            xi1 -= Beta[j - 1] * std::sin(2 * j * xi) * std::cosh(2 * j * eta);
            eta1 -= Beta[j - 1] * std::cos(2 * j * xi) * std::sinh(2 * j * eta);
        }
        const double chi = std::asin(std::sin(xi1) / std::cosh(eta1));
        double phi = chi;
        for (int j = 1; j <= 3; ++j)
        {
            phi += Delta[j - 1] * std::sin(2 * j * chi);
        }
        const double lambda = centralMeridian(utm) + std::atan2(std::sinh(eta1), std::cos(xi1));
        return GeoPosition{ lambda / Rad, phi / Rad };
    }
}
