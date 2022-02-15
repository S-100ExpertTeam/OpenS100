#include "StdAfx.h"
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
	if(Parts != NULL)
		delete[] Parts;
	if(Points != NULL)
		delete[] Points;
	if(CPoints != NULL)
		delete[] CPoints;
}

void GeoPolygon::DrawGeometry(HDC &hDC, Scaler *scaler, double offset)
{
	// Change the map coordinate system to a screen coordinate system and store it in CPpoint.
	for (int i = 0; i< NumPoints; i++) {
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