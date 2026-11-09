// s100::geodesy and the legacy LatLonUtility wrappers that forward to it.
// Reference values: WGS 84 (a = 6378137 m, 1/f = 298.257223563).
#include <gtest/gtest.h>

#include <array>

#include "LatLonUtility/Geodesy.h"
#include "LatLonUtility/LatLonUtility.h"

using namespace s100::geodesy;

namespace
{
	// One degree of longitude on the equator: a * pi / 180.
	constexpr double kEquatorDegree = 111319.4907932736;
	// Meridian arc from the equator to 1 degree north.
	constexpr double kMeridianDegree = 110574.3885577987;
}

TEST(Geodesy, EquatorDegreeIsTheSameOnRhumbAndGeodesic)
{
	const auto rhumb = RhumbInverse({ 0.0, 0.0 }, { 0.0, 1.0 });
	const auto geodesic = GeodesicInverse({ 0.0, 0.0 }, { 0.0, 1.0 });
	EXPECT_NEAR(rhumb.distance, kEquatorDegree, 1e-3);
	EXPECT_NEAR(geodesic.distance, kEquatorDegree, 1e-3);
	EXPECT_NEAR(rhumb.bearing, 90.0, 1e-9);
	EXPECT_NEAR(geodesic.bearing, 90.0, 1e-9);
}

TEST(Geodesy, MeridianDegree)
{
	const auto rhumb = RhumbInverse({ 0.0, 127.0 }, { 1.0, 127.0 });
	EXPECT_NEAR(rhumb.distance, kMeridianDegree, 1e-3);
	EXPECT_NEAR(rhumb.bearing, 0.0, 1e-9);
	EXPECT_NEAR(GeodesicInverse({ 0.0, 127.0 }, { 1.0, 127.0 }).distance, kMeridianDegree, 1e-3);
}

TEST(Geodesy, RhumbIsLongerThanGeodesicAwayFromTheEquator)
{
	// Busan -> Los Angeles: the loxodrome is noticeably longer than the great circle.
	const Position busan{ 35.10, 129.04 };
	const Position losAngeles{ 33.74, -118.27 };
	const auto rhumb = RhumbInverse(busan, losAngeles);
	const auto geodesic = GeodesicInverse(busan, losAngeles);
	EXPECT_GT(rhumb.distance, geodesic.distance + 100000.0);
	EXPECT_NEAR(FromMetres(geodesic.distance, LengthUnit::NauticalMile), 5220.0, 60.0);
}

TEST(Geodesy, DirectAndInverseRoundTrip)
{
	const Position start{ 34.5, 128.25 };
	for (const double bearing : { 0.0, 37.0, 90.0, 181.5, 270.0, 359.0 })
	{
		const auto viaRhumb = RhumbDirect(start, bearing, 25000.0);
		const auto backRhumb = RhumbInverse(start, viaRhumb);
		EXPECT_NEAR(backRhumb.distance, 25000.0, 1e-3) << bearing;
		EXPECT_NEAR(backRhumb.bearing, bearing, 1e-6) << bearing;

		const auto viaGeodesic = GeodesicDirect(start, bearing, 25000.0);
		const auto backGeodesic = GeodesicInverse(start, viaGeodesic);
		EXPECT_NEAR(backGeodesic.distance, 25000.0, 1e-3) << bearing;
		EXPECT_NEAR(backGeodesic.bearing, bearing, 1e-6) << bearing;
	}
}

TEST(Geodesy, BearingIsNormalised)
{
	EXPECT_DOUBLE_EQ(NormalizeBearing(-90.0), 270.0);
	EXPECT_DOUBLE_EQ(NormalizeBearing(360.0), 0.0);
	EXPECT_DOUBLE_EQ(NormalizeBearing(725.0), 5.0);
	// Westwards along the equator.
	EXPECT_NEAR(RhumbInverse({ 0.0, 1.0 }, { 0.0, 0.0 }).bearing, 270.0, 1e-9);
}

TEST(Geodesy, LengthUnits)
{
	EXPECT_DOUBLE_EQ(FromMetres(1852.0, LengthUnit::NauticalMile), 1.0);
	EXPECT_DOUBLE_EQ(FromMetres(1500.0, LengthUnit::Kilometre), 1.5);
	EXPECT_DOUBLE_EQ(ToMetres(2.0, LengthUnit::NauticalMile), 3704.0);
	static_assert(FromMetres(ToMetres(3.0, LengthUnit::Kilometre), LengthUnit::Metre) == 3000.0);
}

TEST(Geodesy, DegreesMinutesSeconds)
{
	EXPECT_NEAR(DmsToDegrees(35, 30, 36), 35.51, 1e-12);
	EXPECT_NEAR(DmsToDegrees(-35, 30, 36), -35.51, 1e-12);

	const auto dms = DegreesToDms(-129.0425);
	EXPECT_DOUBLE_EQ(dms.degrees, -129.0);
	EXPECT_DOUBLE_EQ(dms.minutes, 2.0);
	EXPECT_NEAR(dms.seconds, 33.0, 1e-6);
	EXPECT_NEAR(DmsToDegrees(dms.degrees, dms.minutes, dms.seconds), -129.0425, 1e-12);
}

TEST(Geodesy, PolygonCentroid)
{
	const std::array<double, 4> x{ 0.0, 2.0, 2.0, 0.0 };
	const std::array<double, 4> y{ 0.0, 0.0, 1.0, 1.0 };
	const auto centre = PolygonCentroid(x, y);
	EXPECT_DOUBLE_EQ(centre.lon, 1.0);
	EXPECT_DOUBLE_EQ(centre.lat, 0.5);

	// Clockwise order gives the same point.
	const std::array<double, 4> xr{ 0.0, 0.0, 2.0, 2.0 };
	const std::array<double, 4> yr{ 0.0, 1.0, 1.0, 0.0 };
	const auto reversed = PolygonCentroid(xr, yr);
	EXPECT_DOUBLE_EQ(reversed.lon, 1.0);
	EXPECT_DOUBLE_EQ(reversed.lat, 0.5);

	// A degenerate ring falls back to the vertex average instead of dividing by zero.
	const std::array<double, 2> lx{ 0.0, 4.0 };
	const std::array<double, 2> ly{ 0.0, 2.0 };
	const auto line = PolygonCentroid(lx, ly);
	EXPECT_DOUBLE_EQ(line.lon, 2.0);
	EXPECT_DOUBLE_EQ(line.lat, 1.0);
}

TEST(GeodesyLegacy, WrappersAgreeWithTheTypedApi)
{
	// Rhumb distance: kilometres by default, any unit on request.
	const double km = LatLonUtility::GetDistanceRhumbLineMercator(0.0, 0.0, 0.0, 1.0, false);
	EXPECT_NEAR(km, kEquatorDegree / 1000.0, 1e-6);
	EXPECT_NEAR(LatLonUtility::GetDistanceRhumbLineMercator(0.0, 0.0, 0.0, 1.0, false, LatLonUtility::UNIT::nautical_mile),
		kEquatorDegree / 1852.0, 1e-6);

	// Destination helpers return (lon, lat) in a Pos.
	const auto rhumb = LatLonUtility::GetDestinationPointRhumbLineMercator(0.0, 0.0, 90.0, kEquatorDegree, false);
	EXPECT_NEAR(rhumb.lon, 1.0, 1e-9);
	EXPECT_NEAR(rhumb.lat, 0.0, 1e-9);
	const auto geodesic = LatLonUtility::GetDestinationPointGreateCircle(0.0, 127.0, 0.0, kMeridianDegree, false);
	EXPECT_NEAR(geodesic.lat, 1.0, 1e-9);
	EXPECT_NEAR(geodesic.lon, 127.0, 1e-9);

	double x = 0.0, y = 0.0;
	LatLonUtility::CalcGreatCirclePosByGeographicLib(127.0, 0.0, 0.0, kMeridianDegree, &x, &y);
	EXPECT_NEAR(x, 127.0, 1e-9);
	EXPECT_NEAR(y, 1.0, 1e-9);
	EXPECT_NEAR(LatLonUtility::CalcGreatCircleDistanceByGeographicLib(127.0, 0.0, 127.0, 1.0), kMeridianDegree, 1e-3);

	// Spherical bearing, degrees in (-180, 180].
	EXPECT_NEAR(LatLonUtility::GetBearingGreatCircle(0.0, 0.0, 0.0, 1.0, false), 90.0, 1e-9);
	EXPECT_NEAR(LatLonUtility::GetBearingGreatCircle(0.0, 1.0, 0.0, 0.0, false), -90.0, 1e-9);

	EXPECT_NEAR(LatLonUtility::DMStoDegree(-35, 30, 36), -35.51, 1e-12);
	double d = 0, m = 0, s = 0;
	LatLonUtility::DegreeToDMS(35.51, d, m, s);
	EXPECT_DOUBLE_EQ(d, 35.0);
	EXPECT_DOUBLE_EQ(m, 30.0);
	EXPECT_NEAR(s, 36.0, 1e-6);

	double xs[] = { 0.0, 2.0, 2.0, 0.0 };
	double ys[] = { 0.0, 0.0, 1.0, 1.0 };
	double cx = 0, cy = 0;
	LatLonUtility::Centroid(xs, ys, 4, cx, cy);
	EXPECT_DOUBLE_EQ(cx, 1.0);
	EXPECT_DOUBLE_EQ(cy, 0.5);
}
