#include "stdafx.h"
#include "SPoint.h"

#include "../GeoMetryLibrary/Enum_WKBGeometryType.h"

SPoint::SPoint()
{
	type = 1;
}

SPoint::SPoint(double _x, double _y)
{
	type = 1;
	x = _x;
	y = _y;
}

SPoint::~SPoint()
{

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

bool SPoint::ImportFromWkb(char* value, int size)
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
}

bool SPoint::ExportToWkb(char** value, int* size)
{
	*size = 21;
	if (*value == nullptr)
	{
		*value = new char[*size];
	}
	memset(*value, 0, *size);
	
	(*value)[0] = 0x01;
	
	int type = (int)WKBGeometryType::wkbPoint;
	memcpy_s((*value) + 1, 4, &type, 4);
	memcpy_s((*value) + 5, 8, &x, 8);
	memcpy_s((*value) + 13, 8, &y, 8);
	return true;
}