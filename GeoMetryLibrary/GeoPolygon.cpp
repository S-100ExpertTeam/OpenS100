#include "stdafx.h"
#include "GeoPolygon.h"
#include "GeoPoint.h"
#include "GeoCommonFuc.h"
#include "Scaler.h"

GeoPolygon::GeoPolygon(void)
{
	Parts = NULL;
	Points = NULL;
	CPoints = NULL;
}

GeoPolygon::GeoPolygon(const GeoPolygon& other) : Geometry(other)
{
	NumPoints = other.NumPoints;
	if (NumPoints == 0)
		return;

	Points = new GeoPoint[NumPoints];
	CPoints = new POINT[NumPoints];
	for (int i = 0; i < NumPoints; i++)
	{
		Points[i].id = other.Points[i].id;

		m_mbr.xmin = other.m_mbr.xmin;
		m_mbr.ymin = other.m_mbr.ymin;
		m_mbr.xmax = other.m_mbr.xmax;
		m_mbr.ymax = other.m_mbr.ymax;

		Points[i].x = other.Points[i].x;
		Points[i].y = other.Points[i].y;

		CPoints[i].x = other.CPoints[i].x;
		CPoints[i].y = other.CPoints[i].y;
	}

	NumParts = other.NumParts;
	if (NumParts == 0)
		return;

	Parts = new int[NumParts];
	for (int i = 0; i < NumParts; i++)
	{
		Parts[i] = other.Parts[i];
	}
}

GeoPolygon::~GeoPolygon(void)
{
	if (Parts != NULL)
		delete[] Parts;
	if (Points != NULL)
		delete[] Points;
	if (CPoints != NULL)
		delete[] CPoints;
}

GeoPolygon GeoPolygon::operator=(const GeoPolygon& other)
{
	if (Parts)
	{
		delete[] Parts;
		Parts = nullptr;
	}
		
	if (Points)
	{
		delete[] Points;
		Points = nullptr;
	}
		
	if (CPoints)
	{
		delete[] CPoints;
		CPoints = nullptr;
	}	

	Geometry::operator=(other);

	NumPoints = other.NumPoints;
	if (NumPoints == 0)
		return *this;

	Points = new GeoPoint[NumPoints];
	CPoints = new POINT[NumPoints];
	for (int i = 0; i < NumPoints; i++)
	{
		Points[i].id = other.Points[i].id;

		m_mbr.xmin = other.m_mbr.xmin;
		m_mbr.ymin = other.m_mbr.ymin;
		m_mbr.xmax = other.m_mbr.xmax;
		m_mbr.ymax = other.m_mbr.ymax;

		Points[i].x = other.Points[i].x;
		Points[i].y = other.Points[i].y;

		CPoints[i].x = other.CPoints[i].x;
		CPoints[i].y = other.CPoints[i].y;
	}

	NumParts = other.NumParts;
	if (NumParts == 0)
		return *this;

	Parts = new int[NumParts];
	for (int i = 0; i < NumParts; i++)
	{
		Parts[i] = other.Parts[i];
	}

	return *this;
}

void GeoPolygon::DrawGeometry(HDC& hDC, Scaler* scaler, double offset)
{
	// Change the map coordinate system to a screen coordinate system and store it in CPpoint.
	for (int i = 0; i < NumPoints; i++) {
		scaler->WorldToDevice(Points[i].x + offset, Points[i].y, &CPoints[i].x, &CPoints[i].y);
	}

	auto m_hBrush = ::CreateSolidBrush(RGB(255, 255, 255));
	auto m_hPen = ::CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	::SelectObject(hDC, m_hPen);
	::SelectObject(hDC, m_hBrush);

	// Draw a polygon.
	for (int i = 0; i < NumParts; i++) {
		// last part.
		HRGN hrgn;

		if (i != (NumParts - 1))
		{
			hrgn = ::CreatePolygonRgn(&CPoints[Parts[i]], Parts[i + 1] - Parts[i], WINDING);

			::Polygon(hDC, &CPoints[Parts[i]], Parts[i + 1] - Parts[i]);

		}
		// not last part
		else {
			hrgn = ::CreatePolygonRgn(&CPoints[Parts[i]], NumPoints - Parts[i], WINDING);

			::Polygon(hDC, &CPoints[Parts[i]], NumPoints - Parts[i]);
		}
		DeleteObject(hrgn);

	}
}
