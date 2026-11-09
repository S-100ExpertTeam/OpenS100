#pragma once
// Geodesy on the WGS 84 ellipsoid: rhumb lines (loxodromes), geodesics ("great circles"),
// angle formats and length units.
//
// This is the typed API. The older free functions in LatLonUtility.h
// (GetDistanceRhumbLineMercator, CalcGreatCirclePosByGeographicLib, ...) forward to it and are
// kept for existing callers. Angles are degrees, lengths are metres, bearings are clockwise
// from true north.
#include <span>

namespace s100::geodesy
{
	struct Position
	{
		double lat = 0.0;   ///< degrees, north positive
		double lon = 0.0;   ///< degrees, east positive

		friend bool operator==(const Position&, const Position&) = default;
	};

	/// Result of an inverse problem: how to get from one position to another.
	struct Course
	{
		double distance = 0.0;   ///< metres
		double bearing = 0.0;    ///< degrees in [0, 360); the initial bearing for a geodesic
	};

	enum class LengthUnit
	{
		Metre,
		Kilometre,
		NauticalMile
	};

	inline constexpr double kMetresPerNauticalMile = 1852.0;   // exact, by definition

	/// Converts a length in metres to `unit`.
	constexpr double FromMetres(double metres, LengthUnit unit)
	{
		switch (unit)
		{
		case LengthUnit::Kilometre:    return metres / 1000.0;
		case LengthUnit::NauticalMile: return metres / kMetresPerNauticalMile;
		case LengthUnit::Metre:
		default:                       return metres;
		}
	}

	/// Converts a length given in `unit` to metres.
	constexpr double ToMetres(double value, LengthUnit unit)
	{
		switch (unit)
		{
		case LengthUnit::Kilometre:    return value * 1000.0;
		case LengthUnit::NauticalMile: return value * kMetresPerNauticalMile;
		case LengthUnit::Metre:
		default:                       return value;
		}
	}

	/// Normalises a bearing to [0, 360).
	double NormalizeBearing(double degrees);

	// Rhumb line: constant bearing. What a ship steers and what S-100 route legs default to.
	Course RhumbInverse(Position from, Position to);
	Position RhumbDirect(Position from, double bearing, double distance);

	// Geodesic: the shortest path on the ellipsoid.
	Course GeodesicInverse(Position from, Position to);
	Position GeodesicDirect(Position from, double bearing, double distance);

	/// Degrees/minutes/seconds. The sign is carried by `degrees` only.
	struct Dms
	{
		double degrees = 0.0;
		double minutes = 0.0;
		double seconds = 0.0;
	};

	/// 35, 30, 36 -> 35.51;  -35, 30, 36 -> -35.51
	double DmsToDegrees(double degrees, double minutes, double seconds);
	Dms DegreesToDms(double degrees);

	/// Area-weighted centroid of a simple polygon given as parallel coordinate arrays (the ring
	/// need not be closed). Returns the vertex average for a degenerate (zero-area) ring.
	/// `lon` receives x, `lat` receives y.
	Position PolygonCentroid(std::span<const double> x, std::span<const double> y);
}
