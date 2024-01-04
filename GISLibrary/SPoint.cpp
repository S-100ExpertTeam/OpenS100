#include "stdafx.h"
#include "SPoint.h"

#include "../GeoMetryLibrary/Enum_WKBGeometryType.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"

#include <sstream>

SPoint::SPoint()
{
	
}

SPoint::SPoint(const SPoint& other) : SGeometry(other)
{
	x = other.x;
	y = other.y;
	m_vPoint = other.m_vPoint;
}

SPoint::SPoint(double _x, double _y)
{
	x = _x;
	y = _y;

	SetMBR();
}

SPoint::~SPoint()
{

}

SPoint SPoint::operator=(const SPoint& other)
{
	x = other.x;
	y = other.y;
	m_vPoint = other.m_vPoint;

	return *this;
}

SGeometryType SPoint::GetType()
{
	return SGeometryType::Point;
}

void SPoint::SetPoint(double _x, double _y)
{
	x = _x;
	y = _y;
}

void SPoint::SetMBR()
{
	m_mbr.InitMBR();
	m_mbr.CalcMBR(x, y);
}

bool SPoint::ImportFromWkb(unsigned char* value, int size)
{
	if (value == nullptr || size != 21 ||
		value[0] != 0x01)
	{
		return false;
	}

	int type = 0;

	memcpy_s(&type, 4, value + 1, 4);

	if (type != (int)WKBGeometryType::wkbPoint)
	{
		return false;
	}

	memcpy_s(&x, 8, value + 5, 8);
	memcpy_s(&y, 8, value + 13, 8);

	projection(x, y);

	SetMBR();

	return true;
}

bool SPoint::ExportToWkb(unsigned char** value, int* size)
{
	*size = 21;
	if (*value == nullptr)
	{
		*value = new unsigned char[*size];
	}
	memset(*value, 0, *size);
	
	(*value)[0] = 0x01;
	
	int type = (int)WKBGeometryType::wkbPoint;
	memcpy_s((*value) + 1, 4, &type, 4);

	double localX = x;
	double localY = y;

	inverseProjection(localX, localY);

	memcpy_s((*value) + 5, 8, &localX, 8);
	memcpy_s((*value) + 13, 8, &localY, 8);

	return true;
}

bool SPoint::operator==(const SPoint& point)
{
	if (this->x == point.x && this->y == point.y)
	{
		return true;
	}

	return false;
}

double SPoint::GetX()
{
	return x;
}

double SPoint::GetY()
{
	return y;
}

std::string SPoint::ToString()
{
	std::stringstream ss;
	ss.precision(7);

	double x = GetX();
	double y = GetY();

	inverseProjection(x, y);

	ss << y << " " << x;

	return ss.str();
}

