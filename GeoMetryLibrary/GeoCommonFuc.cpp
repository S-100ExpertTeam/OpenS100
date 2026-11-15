#include "stdafx.h"
#include "GeoCommonFuc.h"
#include "MBR.h"
#include "ENCCommon.h"

#include <algorithm>
#include <cmath>

namespace
{
	MapProjection mapProjection = MapProjection::Mercator;

	constexpr double RadiansToDegrees = 180.0 / M_PI;
	constexpr double MercatorLatitudeLimit = 85.0;

	// The distance from the pole of a latitude given from that pole's side (90 at the pole).
	double polarRadius(double latitude)
	{
		return 2.0 * std::tan((90.0 - latitude) * ex / 2.0);
	}

	// A hemisphere: past the equator the other pole runs off to infinity.
	const double polarRadiusLimit = polarRadius(0.0);
}

void SetMapProjection(MapProjection value)
{
	mapProjection = value;
}

MapProjection GetMapProjection()
{
	return mapProjection;
}

bool IsPolarProjection()
{
	return mapProjection != MapProjection::Mercator;
}

MBR ProjectionLimits()
{
	if (IsPolarProjection())
	{
		return MBR(-polarRadiusLimit, -polarRadiusLimit, polarRadiusLimit, polarRadiusLimit);
	}
	MBR limits(-179.9, -84.9, 179.9, 84.9);
	projection(limits);
	return limits;
}

double NorthAngle(double x, double y)
{
	if (!IsPolarProjection() || (x == 0 && y == 0))
	{
		return 0;
	}
	// North is towards the North Pole, away from the South Pole.
	return mapProjection == MapProjection::PolarStereographicNorth
		? std::atan2(-x, -y) * RadiansToDegrees
		: std::atan2(x, y) * RadiansToDegrees;
}

double ProjectionScaleFactor(double x, double y)
{
	if (IsPolarProjection())
	{
		return 1.0 + (x * x + y * y) / 4.0;
	}
	return std::cosh(y);
}

std::vector<std::pair<double, double>> RhumbLinePointsBetween(double longitude1, double latitude1, double longitude2, double latitude2)
{
	std::vector<std::pair<double, double>> points;
	constexpr double step = 2.0;
	const double turn = longitude2 - longitude1;
	if (!IsPolarProjection() || std::fabs(turn) <= step || std::fabs(turn) > 180.0)
	{
		return points;
	}
	// A rhumb line is straight in Mercator's northing.
	const auto northing = [](double latitude) {
		return std::log(std::tan((45.0 + std::clamp(latitude, -89.99, 89.99) / 2.0) * ex));
	};
	const int parts = static_cast<int>(std::ceil(std::fabs(turn) / step));
	for (int part = 1; part < parts; part++)
	{
		const double t = static_cast<double>(part) / parts;
		const double latitude = latitude1 == latitude2 ? latitude1
			: (2.0 * std::atan(std::exp(northing(latitude1) + (northing(latitude2) - northing(latitude1)) * t)) * RadiansToDegrees - 90.0);
		points.emplace_back(longitude1 + turn * t, latitude);
	}
	return points;
}

//projection
void projection(double &x, double &y)
{
	switch (mapProjection)
	{
	case MapProjection::PolarStereographicNorth:
	{
		const double radius = polarRadius((std::max)(y, 0.0));
		const double longitude = x * ex;
		x = radius * std::sin(longitude);
		y = -radius * std::cos(longitude);
		return;
	}
	case MapProjection::PolarStereographicSouth:
	{
		const double radius = polarRadius((std::max)(-y, 0.0));
		const double longitude = x * ex;
		x = radius * std::sin(longitude);
		y = radius * std::cos(longitude);
		return;
	}
	default:
		break;
	}
	x *= ex;
	y = std::clamp(y, -MercatorLatitudeLimit, MercatorLatitudeLimit) * ex;
	y = std::log(std::tan((y / 2.0) + M_PI_D4));
}


void projection(float &x, float &y)
{
	double _x = x;
	double _y = y;
	projection(_x, _y);
	x = (float)_x;
	y = (float)_y;
}


void projection(MBR& mbr)
{
	if (!IsPolarProjection())
	{
		projection(mbr.xmin, mbr.ymin);
		projection(mbr.xmax, mbr.ymax);
		return;
	}

	// A ring sector about the pole: its box is that of its corners and of the points where its
	// two parallels are furthest left, right, up and down (every quarter turn of longitude).
	const double west = mbr.xmin;
	const double east = (std::min)(mbr.xmax, west + 360.0);
	const double south = mbr.ymin;
	const double north = mbr.ymax;
	MBR box;
	const auto add = [&](double longitude, double latitude)
	{
		projection(longitude, latitude);
		box.CalcMBR(longitude, latitude);
	};
	const auto addMeridian = [&](double longitude)
	{
		add(longitude, south);
		add(longitude, north);
	};
	addMeridian(west);
	addMeridian(east);
	for (double quarter = std::ceil(west / 90.0) * 90.0; quarter < east; quarter += 90.0)
	{
		addMeridian(quarter);
	}
	mbr = box;
}


void inverseProjection(double& x, double& y)
{
	switch (mapProjection)
	{
	case MapProjection::PolarStereographicNorth:
	{
		const double radius = std::hypot(x, y);
		const double longitude = radius > 0 ? std::atan2(x, -y) : 0.0;
		x = longitude * RadiansToDegrees;
		y = 90.0 - 2.0 * std::atan(radius / 2.0) * RadiansToDegrees;
		return;
	}
	case MapProjection::PolarStereographicSouth:
	{
		const double radius = std::hypot(x, y);
		const double longitude = radius > 0 ? std::atan2(x, y) : 0.0;
		x = longitude * RadiansToDegrees;
		y = -90.0 + 2.0 * std::atan(radius / 2.0) * RadiansToDegrees;
		return;
	}
	default:
		break;
	}
	x *= RadiansToDegrees;
	y = (2.0 * std::atan(std::exp(y)) - M_PI / 2.0) * RadiansToDegrees;
}


void inverseProjection(MBR& mbr)
{
	if (!IsPolarProjection())
	{
		inverseProjection(mbr.xmin, mbr.ymin);
		inverseProjection(mbr.xmax, mbr.ymax);
		return;
	}

	const double corners[4][2] = { { mbr.xmin, mbr.ymin }, { mbr.xmin, mbr.ymax }, { mbr.xmax, mbr.ymin }, { mbr.xmax, mbr.ymax } };
	// The point of the box nearest the pole, and its corner furthest from it.
	double nearX = std::clamp(0.0, mbr.xmin, mbr.xmax);
	double nearY = std::clamp(0.0, mbr.ymin, mbr.ymax);
	const bool overPole = nearX == 0 && nearY == 0;
	double farX = 0;
	double farY = 0;
	double longitudeMin = 180.0;
	double longitudeMax = -180.0;
	for (const auto& corner : corners)
	{
		if (std::hypot(corner[0], corner[1]) > std::hypot(farX, farY))
		{
			farX = corner[0];
			farY = corner[1];
		}
		double longitude = corner[0];
		double latitude = corner[1];
		inverseProjection(longitude, latitude);
		longitudeMin = (std::min)(longitudeMin, longitude);
		longitudeMax = (std::max)(longitudeMax, longitude);
	}
	inverseProjection(nearX, nearY);
	inverseProjection(farX, farY);
	// The meridian of 180 degrees runs from the pole along x = 0, up in the north and down in the
	// south: a box over it, as one over the pole, holds longitudes from both of its ends.
	const bool north = mapProjection == MapProjection::PolarStereographicNorth;
	const bool overDateLine = mbr.xmin <= 0 && mbr.xmax >= 0 && (north ? mbr.ymax >= 0 : mbr.ymin <= 0);
	if (overPole || overDateLine)
	{
		longitudeMin = -180.0;
		longitudeMax = 180.0;
	}
	mbr = MBR(longitudeMin, (std::min)(nearY, farY), longitudeMax, (std::max)(nearY, farY));
}


unsigned getClippingLocation(const MBR *mbr, const double &x, const double &y)
{
	if(mbr == NULL)
		return 0;

	unsigned loc = 0;
	if(x < mbr->xmin)
	{
		if(y < mbr->ymin)
		{
			loc = 1;
		}
		else if(y > mbr->ymin && y < mbr->ymax)
		{
			loc = 4;
		}
		else if(y > mbr->ymax)
		{
			loc = 7;
		}
	}
	else if(x > mbr->xmin && x < mbr->xmax)
	{
		if(y < mbr->ymin)
		{
			loc = 2;
		}
		else if(y > mbr->ymax)
		{
			loc = 8;
		}
	}
	else if(x > mbr->xmax)
	{
		if(y < mbr->ymin)
		{
			loc = 3;
		}
		else if(y > mbr->ymin && y < mbr->ymax)
		{
			loc = 6;
		}
		else if(y > mbr->ymax)
		{
			loc = 9;
		}
	}
	return loc;
}