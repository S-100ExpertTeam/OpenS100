#include "stdafx.h"
#include "SCurve.h"
#include "SPoint.h"

#include "../GeoMetryLibrary/Scaler.h"
#include "../GeoMetryLibrary/Enum_WKBGeometryType.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"

#include "../S100Geometry/SPoint.h"

SCurve::SCurve() 
{
	type = 5;
}

SCurve::~SCurve()
{
	delete[] m_pPoints;
	m_pPoints = nullptr;

	SafeRelease(&pGeometry);
}

int SCurve::GetRCID()
{
	int res = (int)m_id;
	return res;
}

int SCurve::GetNumPoints()
{
	return m_numPoints;
}

double SCurve::GetX(int i) 
{
	if (m_pPoints == nullptr)
	{
		return 0;
	}
	return (m_pPoints + i)->x;
}

double SCurve::GetY(int i)
{
	if (m_pPoints == nullptr)
	{
		return 0;
	}
	return (m_pPoints + i)->y;
}

#pragma warning(disable:4244)
void SCurve::CreateD2Geometry(ID2D1Factory1* factory)
{
	if (pGeometry)
	{
		SafeRelease(&pGeometry);
	}

	if (m_numPoints > 1)
	{
		auto points = new D2D1_POINT_2F[m_numPoints];
		for (auto i = 0; i < m_numPoints; i++)
		{
			points[i].x = m_pPoints[i].x;
			points[i].y = -m_pPoints[i].y;
		}

		factory->CreatePathGeometry(&pGeometry);
		ID2D1GeometrySink *pSink = nullptr;
		pGeometry->Open(&pSink);
		pSink->BeginFigure(points[0], D2D1_FIGURE_BEGIN_HOLLOW);
		pSink->AddLines(points + 1, m_numPoints - 1);
		pSink->EndFigure(D2D1_FIGURE_END_OPEN);
		pSink->Close();
		SafeRelease(&pSink);

		delete[] points;
	}
}


ID2D1PathGeometry* SCurve::GetD2Geometry()
{
	return pGeometry;
}


ID2D1PathGeometry* SCurve::GetNewD2Geometry(ID2D1Factory1* factory, Scaler* scaler)
{
	if (m_numPoints > 1)
	{
		ID2D1PathGeometry* newGeometry = nullptr;
		auto points = new D2D1_POINT_2F[m_numPoints];
		for (auto i = 0; i < m_numPoints; i++)
		{
			scaler->WorldToDevice_F(m_pPoints[i].x, m_pPoints[i].y, &points[i].x, &points[i].y); 
		}

		//memory leak
		factory->CreatePathGeometry(&newGeometry);

		ID2D1GeometrySink *pSink = nullptr;
		newGeometry->Open(&pSink);
		pSink->BeginFigure(points[0], D2D1_FIGURE_BEGIN_HOLLOW);
		pSink->AddLines(points + 1, m_numPoints - 1);
		pSink->EndFigure(D2D1_FIGURE_END_OPEN);
		pSink->Close();
		SafeRelease(&pSink);


		delete[] points;
		points = nullptr;
		
		return newGeometry;
	}

	return nullptr;
}

bool SCurve::ImportFromWkb(char* value, int size)
{
	if (value == nullptr ||
		value[0] != 0x01)
	{
		return false;
	}

	int type = 0;

	memcpy_s(&type, 4, value + 1, 4);

	if (type != (int)WKBGeometryType::wkbLineString)
	{
		return false;
	}

	memcpy_s(&m_numPoints, 4, value + 5, 4);

	Init(m_numPoints);

	for (int i = 0; i < m_numPoints; i++)
	{
		memcpy_s(&m_pPoints[i].x, 8, value + (9 + (16 * i)), 8);
		memcpy_s(&m_pPoints[i].y, 8, value + (17 + (16 * i)), 8);

		projection(m_pPoints[i].x, m_pPoints[i].y);
	}

	SetMBR();

	return true;
}

bool SCurve::ExportToWkb(char** value, int* size)
{
	*size = 9 + (16 * m_numPoints);
	if (*value == nullptr)
	{
		*value = new char[*size];
	}
	memset(*value, 0, *size);

	(*value)[0] = 0x01;

	int type = (int)WKBGeometryType::wkbLineString;

	memcpy_s((*value) + 1, 4, &type, 4);

	memcpy_s((*value) + 5, 4, &m_numPoints, 4);

	for (int i = 0; i < m_numPoints; i++)
	{
		double localX = m_pPoints[i].x;
		double localY = m_pPoints[i].y;
		inverseProjection(localX, localY);

		memcpy_s((*value) + (9 + (16 * i)), 8, &localX, 8);
		memcpy_s((*value) + (17 + (16 * i)), 8, &localY, 8);
	}

	return true;
}

int SCurve::WkbSize()
{
	return 9 + (16 * m_numPoints);
}

void SCurve::Init(int size)
{
	SafeRelease(&pGeometry);

	m_id = 0;

	delete[] m_pPoints;
	m_pPoints = new SPoint[size];
	m_numPoints = size;
}

void SCurve::Set(int index, double x, double y)
{
	m_pPoints[index].SetPoint(x, y);
}

void SCurve::SetMBR()
{
	m_mbr.InitMBR();

	for (int i = 0; i < m_numPoints; i++)
	{
		m_mbr.CalcMBR(m_pPoints[i].x, m_pPoints[i].y);
	}
}

void SCurve::Projection()
{
	for (int i = 0; i < m_numPoints; i++)
	{
		projection(m_pPoints[i].x, m_pPoints[i].y);
	}
}

void SCurve::SetMultiplicationFactor(int comfX, int comfY)
{
	for (int i = 0; i < m_numPoints; i++)
	{
		m_pPoints[i].x /= comfX;
		m_pPoints[i].y /= comfY;
	}
}

void SCurve::SetRCID(int value)
{
	m_id = ((__int64)120) << 32 | value;
}