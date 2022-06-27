#include "stdafx.h"
#include "SCurve.h"

#include "../GeoMetryLibrary/Scaler.h"

#include "../S100Geometry/SPoint.h"

SCurve::SCurve() 
{

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
	if (nullptr == pGeometry)
	{
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


