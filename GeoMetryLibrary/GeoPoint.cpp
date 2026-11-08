#include "stdafx.h"
#include "GeoPoint.h"
#include "Scaler.h"

GeoPoint::GeoPoint()
{

}

GeoPoint::GeoPoint(const GeoPoint& other) : Geometry(other)
{
	x = other.x;
	y = other.y;
}

GeoPoint::GeoPoint(double _x, double _y)
{
	x = _x;
	y = _y;
}

GeoPoint::~GeoPoint()
{

}

GeoPoint GeoPoint::operator=(const GeoPoint& other)
{
	Geometry::operator=(other);

	x = other.x;
	y = other.y;

	return *this;
}

void GeoPoint::SetPoint(double _x, double _y)
{
	x = _x;
	y = _y;
}

double GeoPoint::GetX()
{
	return x;
}

double GeoPoint::GetY()
{
	return y;
}

bool GeoPoint::operator == (const GeoPoint& p) const
{
	if (x == p.x && y == p.y)
	{
		return true;
	}

	return false;
}

