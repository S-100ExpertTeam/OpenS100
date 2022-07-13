#include "stdafx.h"
#include "SCompositeCurve.h"
#include "SCurve.h"

#include "../GeoMetryLibrary/Enum_WKBGeometryType.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"

SCompositeCurve::SCompositeCurve()
{
	type = 2;
}

SCompositeCurve::~SCompositeCurve()
{
}

void SCompositeCurve::SetMBR()
{
	m_mbr.InitMBR();

	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		(*i)->SetMBR();
		m_mbr.CalcMBR((*i)->GetMBRRef());
	}
}

void SCompositeCurve::CreateD2Geometry(ID2D1Factory1* factory)
{
	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		(*i)->CreateD2Geometry(factory);
	}
}

bool SCompositeCurve::ImportFromWkb(char* value, int size)
{
	Release();

	if (value == nullptr ||
		value[0] != 0x01)
	{
		return false;
	}

	int type = 0;

	memcpy_s(&type, 4, value + 1, 4);

	if (type == (int)WKBGeometryType::wkbLineString)
	{
		auto curve = new SCurveHasOrient();
		curve->ImportFromWkb(value, size);
		AddCurve(curve);
	}
	else if (type == (int)WKBGeometryType::wkbMultiLineString)
	{
		int numLineStrings = 0;

		memcpy_s(&numLineStrings, 4, value + 5, 4);

		int offset = 0;
		for (int i = 0; i < numLineStrings; i++)
		{
			auto curve = new SCurveHasOrient();
			curve->ImportFromWkb(value + 9 + offset, size - 9 - offset);
			offset += curve->WkbSize();
			AddCurve(curve);
		}

		SetMBR();
	}
	else
	{
		return false;
	}

	return true;
}

bool SCompositeCurve::ExportToWkb(char** value, int* size)
{
	*size = WkbSize();
	if (*value == nullptr)
	{
		*value = new char[*size];
	}
	memset(*value, 0, *size);

	(*value)[0] = 0x01;

	int type = (int)WKBGeometryType::wkbLineString;

	memcpy_s((*value) + 1, 4, &type, 4);

	int numPoints = GetPointCount();
	memcpy_s((*value) + 5, 4, &numPoints, 4);

	for (int i = 0; i < numPoints; i++)
	{
		double localX = 0;
		double localY = 0;

		auto p = GetXY(i);

		inverseProjection(p.x, p.y);

		memcpy_s((*value) + (9 + (16 * i)), 8, &p.x, 8);
		memcpy_s((*value) + (17 + (16 * i)), 8, &p.y, 8);
	}

	return true;
}

bool SCompositeCurve::ExportToWkbMultiLineString(char** value, int* size)
{
	*size = WkbSize();
	if (*value == nullptr)
	{
		*value = new char[*size];
	}
	memset(*value, 0, *size);

	(*value)[0] = 0x01;

	int type = (int)WKBGeometryType::wkbMultiLineString;

	memcpy_s((*value) + 1, 4, &type, 4);

	int numLineStrings = m_listCurveLink.size();
	memcpy_s((*value) + 5, 4, &numLineStrings, 4);

	int offset = 0;
	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		int size = 0;
		auto mem = (*value) + (9 + offset);
		(*i)->ExportToWkb(&mem, &size);
		offset += size;
	}

	return true;
}

int SCompositeCurve::WkbSize()
{
	int size = 9;
	size += (GetPointCount() * 16);
	return size;
}

int SCompositeCurve::WkbSizeMultiLineString()
{
	int size = 9;
	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		size += (*i)->WkbSize();
	}
	return size;
}

void SCompositeCurve::AddCurve(SCurveHasOrient* curve)
{
	m_listCurveLink.push_back(curve);
}

void SCompositeCurve::Release()
{
	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		SafeRelease(&(*i)->pGeometry);
		delete (*i);
	}

	m_listCurveLink.clear();
}

int SCompositeCurve::GetPointCount()
{
	int count = 0;

	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		count += (*i)->GetNumPoints();
	}

	return count;
}

GeoPoint SCompositeCurve::GetXY(int index)
{
	GeoPoint result;
	int pointCount = 0;

	for (auto i = m_listCurveLink.begin(); i != m_listCurveLink.end(); i++)
	{
		int currentPointCount = (*i)->GetNumPoints();
		if (index < pointCount + currentPointCount)
		{
			int localIndex = index - pointCount;
			result.x = (*i)->GetX(localIndex);
			result.y = (*i)->GetY(localIndex);
			return result;
		}

		pointCount += currentPointCount;
	}

	return result;
}