#pragma once

#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

#include <utility>
#include <vector>

class MBR;

static double ex = M_PI / 180.0;
static double limit = -85.0;
static double M_PI_D4 = M_PI / 4.0;
static double EARTH_RADIUS = 6378.1;
static unsigned DOTPERCM = 38;

// The projection of the chart. Everything that is drawn is kept in its world coordinates: a
// position in degrees goes through projection() when a dataset is read, so the datasets are read
// again when the projection changes (CGISLibraryApp::SetMapProjection). All three are conformal,
// on the sphere, in units of the earth's radius.
enum class MapProjection
{
	Mercator,                  // x is the longitude in radians; latitudes to 85 degrees
	PolarStereographicNorth,   // the North Pole at the origin, the Greenwich meridian down; the northern hemisphere
	PolarStereographicSouth,   // the South Pole at the origin, the Greenwich meridian up; the southern hemisphere
};

void SetMapProjection(MapProjection value);
MapProjection GetMapProjection();
// Whether the projection is one about a pole: the world does not repeat in x, and north is not up.
bool IsPolarProjection();
// The world the projection can show.
MBR ProjectionLimits();
// Degrees, clockwise from up, of true north at a world point (0 on Mercator).
double NorthAngle(double x, double y);
// How much the projection enlarges the ground at a world point: world units per radian of
// distance on the ground.
double ProjectionScaleFactor(double x, double y);

// The positions (longitude, latitude in degrees) to draw between the two ends of a segment, which
// is a rhumb line: none where the projection draws it straight (Mercator, or a short segment).
// About a pole a rhumb line is not straight on the chart (along a parallel it is an arc about
// the pole), and a segment over more than a few degrees of longitude gets points between its
// ends. One over more than half a turn is taken to cross the meridian of 180 degrees the short
// way, and gets none.
std::vector<std::pair<double, double>> RhumbLinePointsBetween(double longitude1, double latitude1, double longitude2, double latitude2);

// Degrees of longitude (x) and latitude (y) to world coordinates, and back.
void projection(double &x, double &y);
void projection(float &x, float &y);
void inverseProjection(double& x, double& y);
// A box of longitudes and latitudes to the box of the world that holds it, and a box of the world
// to the box of longitudes and latitudes that holds it. About a pole neither is its corners: a
// world box over the pole holds every longitude.
void projection(MBR& mbr);
void inverseProjection(MBR& mbr);
unsigned getClippingLocation(const MBR *mbr, const double &x, const double &y);
