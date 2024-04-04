#include "stdafx.h"
#include "GeoPointZ.h"

#include <sstream>

#include "Scaler.h"

#include "../GeoMetryLibrary/Enum_WKBGeometryType.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"

GeoPointZ::GeoPointZ(void)
{

}

GeoPointZ::GeoPointZ(const GeoPointZ& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
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

GeoPointZ GeoPointZ::operator=(const GeoPointZ& other)
{
	x = other.x;
	y = other.y;
	z = other.z;

	return *this;
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

bool GeoPointZ::ImportFromWkb(unsigned char* value, int size)
{
	if (value == nullptr || value[0] != 0x01) {
		return false;
	}

	int type = 0;
	memcpy_s(&type, 4, value + 1, 4);

	if (size == 29 && type == (int)WKBGeometryType::wkbPointZ) {
		memcpy_s(&x, 8, value + 5, 8);
		memcpy_s(&y, 8, value + 13, 8);
		memcpy_s(&z, 8, value + 21, 8);
	} 
	else if (size == 21 && type == (int)WKBGeometryType::wkbPoint) {
		memcpy_s(&x, 8, value + 5, 8);
		memcpy_s(&y, 8, value + 13, 8);
		z = 0;
	}
	else {
		return false;
	}

	projection(x, y);

	return true;
}

bool GeoPointZ::ExportToWkb(unsigned char** value, int* size)
{
	*size = 29;
	if (*value == nullptr)
	{
		*value = new unsigned char[*size];
	}
	memset(*value, 0, *size);

	(*value)[0] = 0x01;

	int type = (int)WKBGeometryType::wkbPointZ;
	memcpy_s((*value) + 1, 4, &type, 4);

	memcpy_s((*value) + 5, 8, &x, 8);
	memcpy_s((*value) + 13, 8, &y, 8);
	memcpy_s((*value) + 21, 8, &z, 8);
	return true;
}

std::string GeoPointZ::ToString()
{
	std::stringstream ss;
	ss.precision(7);

	double x = GetX();
	double y = GetY();
	double z = GetZ();

	inverseProjection(x, y);

	ss << y << " " << x << " " << z;

	return ss.str();
}

