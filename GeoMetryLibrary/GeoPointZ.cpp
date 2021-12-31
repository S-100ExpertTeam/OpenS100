#include "StdAfx.h"
#include "GeoPointZ.h"
#include "Scaler.h"

#include <sstream>

GeoPointZ::GeoPointZ(void)
{

}

GeoPointZ::GeoPointZ(double _x, double _y, double _z)
{
	x = _x;
	y = _y;
	z = _z;
}

GeoPointZ::~GeoPointZ(void)
{

}

void GeoPointZ::SetPoint(double _x, double _y, double _z)
{
	x = _x;
	y = _y;
	z = _z;

}
double GeoPointZ::GetZ()
{
	return z;
}

void GeoPointZ::DrawGeometry(HDC &hDC, Scaler *scaler, double offset)
{
	POINT point;
	scaler->WorldToDevice(x + offset, y, &(point.x), &(point.y));

	std::wstringstream streamVal;
	streamVal << z;
	std::wstring strVal = streamVal.str();
	::TextOut(hDC, point.x - 15, point.y - 30, strVal.c_str(), (int)strVal.size());
}