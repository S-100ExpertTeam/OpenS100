#include "stdafx.h"
#include "SMultiPoint.h"

#include "../GeoMetryLibrary/Enum_WKBGeometryType.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"

SMultiPoint::SMultiPoint()
{
	type = 4;
}

SMultiPoint::~SMultiPoint()
{
}

int SMultiPoint::GetNumPoints()
{
	return m_pPoints.size();
}

double SMultiPoint::GetX(int index)
{
	try
	{
		return m_pPoints.at(index).x;
	}
	catch (std::out_of_range)
	{
		return 0;
	}
	return 0;
}

double SMultiPoint::GetY(int index)
{
	try
	{
		return m_pPoints.at(index).y;
	}
	catch (std::out_of_range)
	{
		return 0;
	}

	return 0;
}

double SMultiPoint::GetZ(int index)
{
	try
	{
		return m_pPoints.at(index).z;
	}
	catch (std::out_of_range)
	{
		return 0;
	}

	return 0;
}

void SMultiPoint::SetSize(int size)
{
	m_pPoints.resize(size);
}

void SMultiPoint::SetMBR()
{
	m_mbr.InitMBR();

	for (auto i = m_pPoints.begin(); i != m_pPoints.end(); i++)
	{
		m_mbr.CalcMBR(i->GetX(), i->GetY());
	}
}

void SMultiPoint::Add(double x, double y, double z)
{
	m_pPoints.push_back(GeoPointZ(x, y, z));
}

void SMultiPoint::Set(int index, double x, double y, double z)
{
	try
	{
		return m_pPoints.at(index).SetPoint(x, y, z);
	}
	catch (std::out_of_range)
	{
		return;
	}
}

bool SMultiPoint::ImportFromWkb(unsigned char* value, int size)
{
	if (value == nullptr ||
		value[0] != 0x01)
	{
		return false;
	}

	int type = 0;

	memcpy_s(&type, 4, value + 1, 4);

	if (type != (int)WKBGeometryType::wkbMultiPointZ)
	{
		return false;
	}

	int numPoints = 0;
	memcpy_s(&numPoints, 4, value + 5, 4);

	m_pPoints.resize(numPoints);

	for (int i = 0; i < numPoints; i++)
	{
		m_pPoints.at(i).ImportFromWkb(value + (9 + (29 * i)), 29);
	}

	SetMBR();

	return true;
}

bool SMultiPoint::ExportToWkb(unsigned char** value, int* size)
{
	int numPoints = GetNumPoints();
	*size = 9 + ((29) * numPoints);
	if (*value == nullptr)
	{
		*value = new unsigned char[*size];
	}
	memset(*value, 0, *size);

	(*value)[0] = 0x01;

	int type = (int)WKBGeometryType::wkbMultiPointZ;

	memcpy_s((*value) + 1, 4, &type, 4);

	memcpy_s((*value) + 5, 4, &numPoints, 4);

	for (int i = 0; i < numPoints; i++)
	{
		int locSize = 0;
		auto mem = (*value) + (9 + (29 * i));

		GeoPointZ localPoint = m_pPoints.at(i);
		inverseProjection(localPoint.x, localPoint.y);
		localPoint.ExportToWkb(&mem, &locSize);
	}

	return true;
}

int SMultiPoint::GetType()
{
	return 4;
}