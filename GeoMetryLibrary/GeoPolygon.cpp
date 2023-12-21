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

GeoPolygon::~GeoPolygon(void)
{
	if (Parts != NULL)
		delete[] Parts;
	if (Points != NULL)
		delete[] Points;
	if (CPoints != NULL)
		delete[] CPoints;
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

GeoPolygon* GeoPolygon::Clone() const
{
	GeoPolygon* gp = new GeoPolygon();

	gp->NumPoints = NumPoints;
	if (NumPoints == 0)
		return gp;

	gp->Points = new GeoPoint[NumPoints];
	gp->CPoints = new POINT[NumPoints];
	for (int i = 0; i < NumPoints; i++)
	{
		gp->Points[i].id = Points[i].id;

		gp->m_mbr.xmin = m_mbr.xmin;
		gp->m_mbr.ymin = m_mbr.ymin;
		gp->m_mbr.xmax = m_mbr.xmax;
		gp->m_mbr.ymax = m_mbr.ymax;

		gp->Points[i].x = Points[i].x;
		gp->Points[i].y = Points[i].y;

		gp->CPoints[i].x = CPoints[i].x;
		gp->CPoints[i].y = CPoints[i].y;
	}

	gp->NumParts = NumParts;
	if (NumParts == 0)
		return gp;
	gp->Parts = new int[NumParts];
	for (int i = 0; i < NumParts; i++)
	{
		gp->Parts[i] = Parts[i];
	}

	return gp;
}
