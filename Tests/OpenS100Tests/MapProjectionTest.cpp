// The chart projections (GeoMetryLibrary/GeoCommonFuc.h): Mercator and the stereographic
// projections about the poles.
#include <gtest/gtest.h>

#include <cmath>
#include <utility>

#include "../GeoMetryLibrary/GeoCommonFuc.h"
#include "../GeoMetryLibrary/MBR.h"
#include "../GeoMetryLibrary/Scaler.h"

namespace
{
	// The projection is one for the process: every test leaves Mercator behind.
	struct ProjectionScope
	{
		explicit ProjectionScope(MapProjection value) { SetMapProjection(value); }
		~ProjectionScope() { SetMapProjection(MapProjection::Mercator); }
	};

	constexpr double Pi = 3.14159265358979323846;
}

TEST(MapProjection, MercatorStretchesTheLatitudes)
{
	ProjectionScope scope(MapProjection::Mercator);
	EXPECT_FALSE(IsPolarProjection());

	double x = 129.0;
	double y = 35.0;
	projection(x, y);
	EXPECT_NEAR(x, 129.0 * Pi / 180.0, 1e-12);
	EXPECT_NEAR(y, std::log(std::tan(Pi / 4 + 35.0 * Pi / 360.0)), 1e-12);
	EXPECT_NEAR(ProjectionScaleFactor(x, y), 1.0 / std::cos(35.0 * Pi / 180.0), 1e-12);
	EXPECT_EQ(NorthAngle(x, y), 0.0);

	inverseProjection(x, y);
	EXPECT_NEAR(x, 129.0, 1e-9);
	EXPECT_NEAR(y, 35.0, 1e-9);

	// Past 85 degrees the latitude stays at 85: the pole is at infinity.
	double poleX = 0;
	double poleY = 90.0;
	projection(poleX, poleY);
	inverseProjection(poleX, poleY);
	EXPECT_NEAR(poleY, 85.0, 1e-9);
}

TEST(MapProjection, NorthPolarPutsThePoleAtTheOriginAndGreenwichDown)
{
	ProjectionScope scope(MapProjection::PolarStereographicNorth);
	EXPECT_TRUE(IsPolarProjection());

	double x = 37.0;
	double y = 90.0;
	projection(x, y);
	EXPECT_NEAR(x, 0.0, 1e-12);
	EXPECT_NEAR(y, 0.0, 1e-12);
	EXPECT_NEAR(ProjectionScaleFactor(x, y), 1.0, 1e-12);

	// 80N on the Greenwich meridian is straight down from the pole, 90E to the right.
	const double radius = 2.0 * std::tan(5.0 * Pi / 180.0);
	x = 0.0;
	y = 80.0;
	projection(x, y);
	EXPECT_NEAR(x, 0.0, 1e-12);
	EXPECT_NEAR(y, -radius, 1e-12);
	EXPECT_NEAR(NorthAngle(x, y), 0.0, 1e-9);
	x = 90.0;
	y = 80.0;
	projection(x, y);
	EXPECT_NEAR(x, radius, 1e-12);
	EXPECT_NEAR(y, 0.0, 1e-12);
	// North, towards the pole, is to the left there.
	EXPECT_NEAR(NorthAngle(x, y), -90.0, 1e-9);

	for (const auto& [lon, lat] : { std::pair{ -170.0, 62.5 }, std::pair{ 15.0, 89.0 }, std::pair{ 179.5, 10.0 } })
	{
		double px = lon;
		double py = lat;
		projection(px, py);
		inverseProjection(px, py);
		EXPECT_NEAR(px, lon, 1e-9);
		EXPECT_NEAR(py, lat, 1e-9);
	}

	// The southern hemisphere is not shown: it stays on the equator.
	x = 20.0;
	y = -40.0;
	projection(x, y);
	inverseProjection(x, y);
	EXPECT_NEAR(y, 0.0, 1e-9);
}

TEST(MapProjection, SouthPolarPutsThePoleAtTheOriginAndGreenwichUp)
{
	ProjectionScope scope(MapProjection::PolarStereographicSouth);

	double x = 0.0;
	double y = -80.0;
	projection(x, y);
	EXPECT_NEAR(x, 0.0, 1e-12);
	EXPECT_NEAR(y, 2.0 * std::tan(5.0 * Pi / 180.0), 1e-12);
	EXPECT_NEAR(NorthAngle(x, y), 0.0, 1e-9);

	x = 90.0;
	y = -80.0;
	projection(x, y);
	EXPECT_GT(x, 0.0);
	// North, away from the pole, is to the right there.
	EXPECT_NEAR(NorthAngle(x, y), 90.0, 1e-9);

	x = -120.0;
	y = -71.0;
	projection(x, y);
	inverseProjection(x, y);
	EXPECT_NEAR(x, -120.0, 1e-9);
	EXPECT_NEAR(y, -71.0, 1e-9);
}

TEST(MapProjection, BoxesAboutAPoleAreNotTheirCorners)
{
	ProjectionScope scope(MapProjection::PolarStereographicNorth);
	const double radius = 2.0 * std::tan(5.0 * Pi / 180.0);   // of 80N

	// The cap north of 80N is a disc about the origin, though its corners are one point.
	MBR cap(-180.0, 80.0, 180.0, 90.0);
	projection(cap);
	EXPECT_NEAR(cap.xmin, -radius, 1e-12);
	EXPECT_NEAR(cap.xmax, radius, 1e-12);
	EXPECT_NEAR(cap.ymin, -radius, 1e-12);
	EXPECT_NEAR(cap.ymax, radius, 1e-12);

	// A world box over the pole holds every longitude, up to the pole.
	MBR overPole(-0.01, -0.01, 0.02, 0.01);
	inverseProjection(overPole);
	EXPECT_EQ(overPole.xmin, -180.0);
	EXPECT_EQ(overPole.xmax, 180.0);
	EXPECT_NEAR(overPole.ymax, 90.0, 1e-9);
	EXPECT_LT(overPole.ymin, 90.0);

	// One beside the pole holds the longitudes and latitudes of what is in it.
	MBR world(30.0, 70.0, 60.0, 80.0);
	projection(world);
	inverseProjection(world);
	EXPECT_LE(world.xmin, 30.0 + 1e-9);
	EXPECT_GE(world.xmax, 60.0 - 1e-9);
	EXPECT_LE(world.ymin, 70.0 + 1e-9);
	EXPECT_GE(world.ymax, 80.0 - 1e-9);
	EXPECT_GT(world.xmin, -180.0);
	EXPECT_LT(world.xmax, 180.0);

	// One over the meridian of 180 degrees holds both of its ends.
	MBR dateLine(170.0, 70.0, 190.0, 80.0);
	projection(dateLine);
	inverseProjection(dateLine);
	EXPECT_EQ(dateLine.xmin, -180.0);
	EXPECT_EQ(dateLine.xmax, 180.0);
}

TEST(MapProjection, TheScaleIsThatOfTheGroundAtTheCentreOfTheView)
{
	// The same 0.2 degrees of longitude are half as long on the ground at 60N as at the equator:
	// a screen that shows them is at twice the scale.
	const auto scaleOf = [](double latitude) {
		Scaler scaler;
		scaler.SetScreen(0, 0, 999, 599);
		double west = 10.0;
		double south = latitude;
		double east = 10.2;
		double north = latitude;
		projection(west, south);
		projection(east, north);
		scaler.SetMap(west, south - 0.001, east, north + 0.001);
		return scaler.GetCurrentScale();
	};
	{
		ProjectionScope scope(MapProjection::Mercator);
		EXPECT_NEAR(static_cast<double>(scaleOf(60.0)) / scaleOf(0.0), 0.5, 0.01);

		// SetScale keeps the centre and gives the scale asked for.
		Scaler scaler;
		scaler.SetScreen(0, 0, 999, 599);
		double x = 129.0;
		double y = 35.0;
		projection(x, y);
		scaler.SetMap(x - 0.01, y - 0.01, x + 0.01, y + 0.01);
		scaler.SetScale(50000);
		EXPECT_NEAR(scaler.GetCurrentScale(), 50000, 5);
		EXPECT_NEAR((scaler.mxMin + scaler.mxMax) / 2, x, 1e-12);
		EXPECT_NEAR((scaler.myMin + scaler.myMax) / 2, y, 1e-12);
	}
	{
		ProjectionScope scope(MapProjection::PolarStereographicNorth);
		Scaler scaler;
		scaler.SetScreen(0, 0, 999, 599);
		const MBR limits = ProjectionLimits();
		EXPECT_EQ(scaler.mxMinLimit, limits.xmin);
		double x = 0.0;
		double y = 89.0;
		projection(x, y);
		scaler.SetMap(x - 0.01, y - 0.01, x + 0.01, y + 0.01);
		scaler.SetScale(50000);
		EXPECT_NEAR(scaler.GetCurrentScale(), 50000, 5);
	}
}

// A turned view (Scaler::SetRotationDegree): the picture is turned clockwise about the centre
// of the screen, and the two directions of the transform undo each other.
TEST(ScalerRotation, TurnsThePictureClockwiseAboutTheCentreOfTheScreen)
{
	ProjectionScope scope(MapProjection::Mercator);
	Scaler scaler;
	scaler.SetScreen(0, 0, 1000, 600);
	double x = 129.0;
	double y = 35.0;
	projection(x, y);
	scaler.SetMap(x - 0.01, y - 0.006, x + 0.01, y + 0.006);

	// Unturned: a point north of the centre is straight up.
	float sx = 0;
	float sy = 0;
	scaler.WorldToDevice_F(x, y + 0.001, &sx, &sy);
	const float up = 300 - sy;
	EXPECT_NEAR(sx, 500, 1e-3);
	EXPECT_GT(up, 10);

	// Turned 90 degrees, north is to the right; the centre stays.
	scaler.SetRotationDegree(90);
	scaler.WorldToDevice_F(x, y, &sx, &sy);
	EXPECT_NEAR(sx, 500, 1e-3);
	EXPECT_NEAR(sy, 300, 1e-3);
	scaler.WorldToDevice_F(x, y + 0.001, &sx, &sy);
	EXPECT_NEAR(sx, 500 + up, 1e-2);
	EXPECT_NEAR(sy, 300, 1e-2);
	EXPECT_NEAR(scaler.NorthOnScreen(x, y), 90, 1e-12);

	// With rotate false the screen is as it would be with north up.
	scaler.WorldToDevice_F(x, y + 0.001, &sx, &sy, false);
	EXPECT_NEAR(sx, 500, 1e-3);
	EXPECT_NEAR(sy, 300 - up, 1e-2);

	// The degrees are kept from 0 to 359.
	scaler.SetRotationDegree(-30);
	EXPECT_EQ(scaler.GetRotationDegree(), 330);
	scaler.Rotate(40);
	EXPECT_EQ(scaler.GetRotationDegree(), 10);
	scaler.SetRotationDegree(360);
	EXPECT_EQ(scaler.GetRotationDegree(), 0);
}

TEST(ScalerRotation, ScreenToMapUndoesMapToScreen)
{
	ProjectionScope scope(MapProjection::Mercator);
	Scaler scaler;
	scaler.SetScreen(0, 0, 1000, 600);
	double x = 129.0;
	double y = 35.0;
	projection(x, y);
	scaler.SetMap(x - 0.01, y - 0.006, x + 0.01, y + 0.006);
	const double pixel = 0.02 / 1000;

	for (int degrees : { 0, 37, 90, 180, 233, 330 })
	{
		scaler.SetRotationDegree(degrees);
		for (const auto& [px, py] : { std::pair<long, long>{ 0, 0 }, { 1000, 600 }, { 123, 456 }, { 500, 300 } })
		{
			double mx = 0;
			double my = 0;
			scaler.DeviceToWorld(px, py, &mx, &my);
			float sx = 0;
			float sy = 0;
			scaler.WorldToDevice_F(mx, my, &sx, &sy);
			EXPECT_NEAR(sx, px, 1e-2) << degrees;
			EXPECT_NEAR(sy, py, 1e-2) << degrees;

			// What the view shows holds every point of the screen.
			const MBR map = scaler.GetMap();
			EXPECT_GE(mx, map.xmin - pixel) << degrees;
			EXPECT_LE(mx, map.xmax + pixel) << degrees;
			EXPECT_GE(my, map.ymin - pixel) << degrees;
			EXPECT_LE(my, map.ymax + pixel) << degrees;
		}
	}

	// A point of the map put at a point of the screen is there, turned or not; and a rectangle
	// of the screen made the view keeps its middle.
	scaler.SetRotationDegree(37);
	scaler.MoveMap(200, 100, x, y);
	float sx = 0;
	float sy = 0;
	scaler.WorldToDevice_F(x, y, &sx, &sy);
	EXPECT_NEAR(sx, 200, 1e-2);
	EXPECT_NEAR(sy, 100, 1e-2);

	double middleX = 0;
	double middleY = 0;
	scaler.DeviceToWorld(300, 200, &middleX, &middleY);
	scaler.SetMapFromScreen(200, 140, 400, 260);
	scaler.WorldToDevice_F(middleX, middleY, &sx, &sy);
	EXPECT_NEAR(sx, 500, 0.5);
	EXPECT_NEAR(sy, 300, 0.5);
}
