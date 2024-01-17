#include "stdafx.h"
#include "GeoMultiPoint.h"
#include "Scaler.h"
#include "GeoPoint.h"

GeoMultiPoint::GeoMultiPoint()
{

}

GeoMultiPoint::GeoMultiPoint(const GeoMultiPoint& other) : Geometry(other)
{
	m_numPoints = other.m_numPoints;
	if (m_numPoints == 0)
		return;

	m_pPoints = new GeoPoint[m_numPoints];
	for (int i = 0; i < m_numPoints; i++)
	{
		m_pPoints[i].x = other.m_pPoints[i].x;
		m_pPoints[i].y = other.m_pPoints[i].y;
	}
}

GeoMultiPoint::GeoMultiPoint(int size)
{
	m_numPoints = size;
	m_pPoints = new GeoPoint[size];
}

GeoMultiPoint::GeoMultiPoint(double _x, double _y)
{
	m_numPoints = 1;
	m_pPoints = new GeoPoint[m_numPoints];
	m_pPoints[0].SetPoint(_x, _y);
}

#pragma warning(disable:4018)
GeoMultiPoint::GeoMultiPoint(int size, double* _x, double* _y)
{
	m_numPoints = size;
	m_pPoints = new GeoPoint[m_numPoints];

	for (unsigned i = 0; i < m_numPoints; i++)
	{
		m_pPoints[i].SetPoint(_x[i], _y[i]);
	}
}
GeoMultiPoint::~GeoMultiPoint()
{
	delete[] m_pPoints;
	m_pPoints = nullptr;
}

GeoMultiPoint GeoMultiPoint::operator=(const GeoMultiPoint& other)
{
	if (m_pPoints)
	{
		delete[] m_pPoints;
		m_pPoints = nullptr;
	}

	Geometry::operator=(other);

	m_numPoints = other.m_numPoints;
	if (m_numPoints == 0)
		return *this;

	m_pPoints = new GeoPoint[m_numPoints];
	for (int i = 0; i < m_numPoints; i++)
	{
		m_pPoints[i].x = other.m_pPoints[i].x;
		m_pPoints[i].y = other.m_pPoints[i].y;
	}

	return *this;
}

void GeoMultiPoint::SetPoint(int index, double _x, double _y)
{
	m_pPoints[index].SetPoint(_x, _y);
}

void GeoMultiPoint::DrawGeometry(HDC& hDC, Scaler* scaler, double offset)
{
	POINT point;
	HBRUSH m_hBrush = (HBRUSH)GetStockObject(RGB(255, 191, 221));

	for (unsigned i = 0; i < m_numPoints; i++)
	{
		scaler->WorldToDevice(m_pPoints[i].x + offset, m_pPoints[i].y, &(point.x), &(point.y));

		RECT r;
		r.left = point.x - 2;
		r.top = point.y - 2;
		r.right = point.x + 2;
		r.bottom = point.y + 2;
		::FillRect(hDC, &r, m_hBrush);
	}
}

