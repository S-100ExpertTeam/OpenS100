#include "Geodesy.h"

#include <algorithm>
#include <cmath>

#include <GeographicLib/Constants.hpp>
#include <GeographicLib/Geodesic.hpp>
#include <GeographicLib/Rhumb.hpp>

namespace s100::geodesy
{
	namespace
	{
		const GeographicLib::Rhumb& Wgs84Rhumb()
		{
			static const GeographicLib::Rhumb rhumb(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f());
			return rhumb;
		}

		const GeographicLib::Geodesic& Wgs84Geodesic()
		{
			return GeographicLib::Geodesic::WGS84();
		}
	}

	double NormalizeBearing(double degrees)
	{
		double bearing = std::fmod(degrees, 360.0);
		if (bearing < 0.0)
		{
			bearing += 360.0;
		}
		return bearing == 360.0 ? 0.0 : bearing;
	}

	Course RhumbInverse(Position from, Position to)
	{
		double distance = 0.0;
		double azimuth = 0.0;
		Wgs84Rhumb().Inverse(from.lat, from.lon, to.lat, to.lon, distance, azimuth);
		return { distance, NormalizeBearing(azimuth) };
	}

	Position RhumbDirect(Position from, double bearing, double distance)
	{
		Position result;
		Wgs84Rhumb().Direct(from.lat, from.lon, bearing, distance, result.lat, result.lon);
		return result;
	}

	Course GeodesicInverse(Position from, Position to)
	{
		double distance = 0.0;
		double azimuthAtStart = 0.0;
		double azimuthAtEnd = 0.0;
		Wgs84Geodesic().Inverse(from.lat, from.lon, to.lat, to.lon, distance, azimuthAtStart, azimuthAtEnd);
		return { distance, NormalizeBearing(azimuthAtStart) };
	}

	Position GeodesicDirect(Position from, double bearing, double distance)
	{
		Position result;
		Wgs84Geodesic().Direct(from.lat, from.lon, bearing, distance, result.lat, result.lon);
		return result;
	}

	double DmsToDegrees(double degrees, double minutes, double seconds)
	{
		const double magnitude = std::abs(degrees) + minutes / 60.0 + seconds / 3600.0;
		return std::signbit(degrees) ? -magnitude : magnitude;
	}

	Dms DegreesToDms(double degrees)
	{
		const double magnitude = std::abs(degrees);
		Dms result;
		result.degrees = std::floor(magnitude);
		const double minutes = (magnitude - result.degrees) * 60.0;
		result.minutes = std::floor(minutes);
		result.seconds = (minutes - result.minutes) * 60.0;
		if (degrees < 0.0)
		{
			result.degrees = -result.degrees;
		}
		return result;
	}

	Position PolygonCentroid(std::span<const double> x, std::span<const double> y)
	{
		const std::size_t count = std::min(x.size(), y.size());
		if (count == 0)
		{
			return {};
		}

		double twiceArea = 0.0;
		double cx = 0.0;
		double cy = 0.0;
		double sumX = 0.0;
		double sumY = 0.0;
		for (std::size_t i = 0; i < count; ++i)
		{
			const std::size_t j = (i + 1) % count;
			const double cross = x[i] * y[j] - x[j] * y[i];
			twiceArea += cross;
			cx += (x[i] + x[j]) * cross;
			cy += (y[i] + y[j]) * cross;
			sumX += x[i];
			sumY += y[i];
		}

		if (std::abs(twiceArea) < 1e-300)
		{
			// Degenerate ring (a point or a line): the shoelace formula would divide by zero.
			return { sumY / static_cast<double>(count), sumX / static_cast<double>(count) };
		}
		return { cy / (3.0 * twiceArea), cx / (3.0 * twiceArea) };
	}
}
