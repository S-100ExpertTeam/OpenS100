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

GeoPolygon* GeoPolygon::Clip(int left, int bottom, int right, int top)
{

	ClipperLib::Path  polygon, view;
	ClipperLib::Paths result;
	ClipperLib::Clipper  clipper;

	ClipperLib::IntPoint tmp;

	tmp.X = left;   // Specify the coordinates of the View port
	tmp.Y = top;    //
	view.push_back(tmp);	   //
	tmp.X = right;  //
	tmp.Y = top;	   //
	view.push_back(tmp);	   //
	tmp.X = right;  //
	tmp.Y = bottom; //
	view.push_back(tmp);	   //
	tmp.X = left;   //
	tmp.Y = bottom; //
	view.push_back(tmp);	   //

	int part1PointCount;											// Find the number of points for the first part of the polygon.
	//
	if (NumParts > 1) 									//
	{																//
		part1PointCount = Parts[1] - Parts[0];	//
	}																//
	else 															//
	{																//
		part1PointCount = NumPoints;						//
	}																//

	for (int i = 0; i < part1PointCount; i++)
	{
		tmp.X = CPoints[i].x;
		tmp.Y = CPoints[i].y;
		polygon.push_back(tmp);
	}

	clipper.AddPath(polygon, ClipperLib::ptSubject, true);
	clipper.AddPath(view, ClipperLib::ptClip, true);
	clipper.Execute(ClipperLib::ctIntersection, result, ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);

	if (result.size() == 0)
	{
		return NULL;
	}

	GeoPolygon *returnValue = new GeoPolygon();
	returnValue->NumParts = (int)result.size();
	returnValue->Parts = new int[result.size()];
	returnValue->NumPoints = 0;

	for (unsigned int i = 0; i < result.size(); i++)
	{
		returnValue->NumPoints += (int)result[i].size();
	}
	returnValue->CPoints = new POINT[returnValue->NumPoints];

	returnValue->Parts[0] = 0;
	for (unsigned int i = 1; i < result.size(); i++)
	{
		returnValue->Parts[i] = returnValue->Parts[i - 1] + (int)result[i - 1].size();
	}

	int pointCount = 0;
	for (unsigned int i = 0; i < result.size(); i++)
	{
		for (unsigned int j = 0; j < result[i].size(); j++)
		{
			returnValue->CPoints[pointCount].x = (LONG)(result[i][j].X);
			returnValue->CPoints[pointCount].y = (LONG)(result[i][j].Y);
			pointCount++;
		}
	}

	return returnValue;
}

void GeoPolygon::DrawGeometry(CDCRenderTarget* pRenderTarget, Scaler *scaler, double offsetX, double offsetY)
{
	// Ready to coordinate the polygon screen.
	long* sx = new long[NumPoints];
	long* sy = new long[NumPoints];

	for (int i = 0; i < NumPoints; i++)
	{
		double mx = Points[i].x;
		double my = Points[i].y;

		projection(mx, my);
		scaler->WorldToDevice(mx, my, &sx[i], &sy[i]);
	}

	// Draw a polyline.
	CD2DPathGeometry* m_pGeometry = new CD2DPathGeometry(pRenderTarget);

	m_pGeometry->Create(pRenderTarget);
	CD2DGeometrySink m_pSink(*m_pGeometry);

	m_pSink.SetFillMode(D2D1_FILL_MODE_WINDING);
	m_pSink.BeginFigure(CD2DPointF((FLOAT)(sx[0] + offsetX), (FLOAT)(sy[0] + offsetY)), D2D1_FIGURE_BEGIN_FILLED);

	for (int i = 1; i < NumPoints; i++)
	{
		m_pSink.AddLine(CD2DPointF((FLOAT)(sx[i] + offsetX), (FLOAT)(sy[i] + offsetY)));
	}

	m_pSink.EndFigure(D2D1_FIGURE_END_OPEN);
	m_pSink.Close();

	CD2DSolidColorBrush* m_pBlackBrush = new CD2DSolidColorBrush(pRenderTarget, D2D1::ColorF(D2D1::ColorF::Crimson));
	m_pBlackBrush->SetOpacity((FLOAT)0.2);
	pRenderTarget->DrawGeometry(m_pGeometry, m_pBlackBrush, 2);
	m_pBlackBrush->SetOpacity((FLOAT)0.7);

	delete m_pGeometry;

	delete[] sx;
	delete[] sy;
}