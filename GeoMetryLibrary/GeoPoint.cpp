#include "StdAfx.h"
#include "GeoPoint.h"
#include "Scaler.h"

GeoPoint::GeoPoint()
{

}

GeoPoint::GeoPoint(double _x, double _y)
{
	x = _x;
	y = _y;
}

GeoPoint::~GeoPoint()
{

}

void GeoPoint::SetPoint(double _x, double _y)
{
	x = _x;
	y = _y;
}

void GeoPoint::DrawGeometry(HDC &hDC, Scaler *scaler, double offset)
{
	POINT point;
	scaler->WorldToDevice(x + offset, y, &(point.x), &(point.y));
	Ellipse(hDC, point.x - 1, point.y - 1, point.x + 1, point.y + 1);
}

double GeoPoint::GetX()
{
	return x;
}

double GeoPoint::GetY()
{
	return y;
}

bool GeoPoint::operator == (GeoPoint& p)
{
	if (x == p.x && y == p.y)
	{
		return true;
	}

	return false;
}