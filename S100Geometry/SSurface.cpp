#include "stdafx.h"
#include "SSurface.h"
#include "SCommonFuction.h"
#include "SCurve.h"

#include "..\\GeoMetryLibrary\\MBR.h"
#include "..\\GeoMetryLibrary\\Scaler.h"
#include "..\\GeoMetryLibrary\\GeoCommonFuc.h"

SSurface::SSurface()
{
	type = 3;
}

SSurface::SSurface(MBR* mbr)
{
	type = 3;
	m_numParts = 1;
	m_numPoints = 5;
	m_pParts = new int[1];
	m_pParts[0] = 0;
	m_pPoints = new GeoPoint[5];
	m_pPoints[0].x = mbr->xmin;
	m_pPoints[0].y = mbr->ymin;
	m_pPoints[1].x = mbr->xmax;
	m_pPoints[1].y = mbr->ymin;
	m_pPoints[2].x = mbr->xmax;
	m_pPoints[2].y = mbr->ymax;
	m_pPoints[3].x = mbr->xmin;
	m_pPoints[3].y = mbr->ymax;
	m_pPoints[4].x = mbr->xmin;
	m_pPoints[4].y = mbr->ymin;
	m_mbr.xmin = mbr->xmin;
	m_mbr.ymin = mbr->ymin;
	m_mbr.xmax = mbr->xmax;
	m_mbr.ymax = mbr->ymax;
}

SSurface::SSurface(std::vector<POINT>& points, std::vector<int> parts)
{
	type = 3;
	m_numParts = (int)parts.size();
	m_numPoints = (int)points.size();

	if (m_numParts < 2)
	{
		m_pParts = new int[1];
		m_pParts[0] = 0;
	}
	else
	{
		m_pParts = new int[m_numParts];
		m_pParts[0] = 0;
		int size = 0;
		size = (int)parts.size();
		for (int i = 1; i < size ; i++)
		{
			m_pParts[i] = parts.at(i - 1);
		}
	}

	m_pPoints = new GeoPoint[m_numPoints];

	if (m_numPoints > SGeometry::sizeOfPoint)
	{
		SGeometry::sizeOfPoint = m_numPoints;
		delete SGeometry::viewPoints;
		SGeometry::viewPoints = new CPoint[int(SGeometry::sizeOfPoint * 1.5)];
	}

	for (int i = 0; i < m_numPoints; i++)
	{
		m_pPoints[i].SetPoint(points[i].x / 10000000.0, points[i].y / 10000000.0);
		projection(m_pPoints[i].x, m_pPoints[i].y);
		m_mbr.CalcMBR(m_pPoints[i].x, m_pPoints[i].y);
	}
}

SSurface::~SSurface()
{
	delete[] m_pPoints;
	m_pPoints = nullptr;

	delete[] m_pParts;
	m_pParts = nullptr;

	delete m_centerPoint;
	m_centerPoint = nullptr;

	SafeRelease(&pGeometry);
}


#pragma warning(disable:4244)
void SSurface::CreateD2Geometry(ID2D1Factory1* factory)
{
	if (nullptr == pGeometry &&
		m_numPoints > 2)
	{
		auto points = new D2D1_POINT_2F[m_numPoints];

		for (int i = 0; i < m_numPoints; i++)
		{
			points[i].x = m_pPoints[i].x;
			points[i].y = -m_pPoints[i].y;
		}

		if (m_numParts == 1)
		{
			auto result = factory->CreatePathGeometry((ID2D1PathGeometry **)&pGeometry);
			if (SUCCEEDED(result))
			{
				ID2D1PathGeometry *pPathGeometry = (ID2D1PathGeometry *)pGeometry;
				ID2D1GeometrySink *pSink = nullptr;
				result = pPathGeometry->Open(&pSink);
				if (SUCCEEDED(result))
				{
					pSink->BeginFigure(points[0], D2D1_FIGURE_BEGIN_FILLED);
					pSink->AddLines(&points[1], m_numPoints - 1);
					pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
					result = pSink->Close();
					if (SUCCEEDED(result))
					{
						SafeRelease(&pSink);
					}
				}
			}
		}
		else if (m_numParts > 1)
		{
			// Area
			ID2D1PathGeometry **segGeometry = new ID2D1PathGeometry*[m_numParts];
			for (int i = 0; i < m_numParts; i++)
			{
				auto result = factory->CreatePathGeometry(&segGeometry[i]);
				if (SUCCEEDED(result))
				{
					ID2D1GeometrySink *pSink = nullptr;
					result = segGeometry[i]->Open(&pSink);
					if (SUCCEEDED(result))
					{
						pSink->BeginFigure(points[m_pParts[i]], D2D1_FIGURE_BEGIN_FILLED);
						pSink->AddLines(&points[m_pParts[i] + 1], GetNumPointPerPart(i) - 1);
						pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
						result = pSink->Close();
						if (SUCCEEDED(result))
						{
							SafeRelease(&pSink);
						}
					}

				}
			}

			auto result = factory->CreateGeometryGroup(
				D2D1_FILL_MODE_ALTERNATE,
				(ID2D1Geometry **)segGeometry,
				m_numParts,
				(ID2D1GeometryGroup **)&(pGeometry));

			for (int i = 0; i < m_numParts; i++)
			{
				SafeRelease(&segGeometry[i]);
			}

			delete[] segGeometry;
			segGeometry = nullptr;
		}

		delete[] points;
		points = nullptr;

		for (auto i = curveList.begin(); i != curveList.end(); i++)
		{
			(*i).GetCurve()->CreateD2Geometry(factory);
		}
	}
}


int SSurface::GetNumPointPerPart(int partIndex)
{
	if (partIndex < 0 || partIndex >= m_numParts)
	{
		return 0;
	}

	if (partIndex == m_numParts - 1)
	{
		return m_numPoints - m_pParts[partIndex];
	}

	return m_pParts[partIndex + 1] - m_pParts[partIndex];
}

ID2D1PathGeometry* SSurface::GetD2Geometry()
{
	return pGeometry;
}

void SSurface::CalculateCenterPoint()
{
	int j = 0;
	double area = 0;
	double centerX = 0.0;
	double centerY = 0.0;

	double x1, y1, x2, y2, tmpArea;

	for (int i = 0; i < m_numPoints; i++)
	{
		j = (i + 1) % m_numPoints;

		x1 = m_pPoints[i].x;
		y1 = m_pPoints[i].y;
		x2 = m_pPoints[j].x;
		y2 = m_pPoints[j].y;

		tmpArea = ((x1 * y2) - (x2 * y1));

		centerX += ((x1 + x2) * tmpArea);
		centerY += ((y1 + y2) * tmpArea);
		area += tmpArea;
	}

	area *= 0.5;

	centerX = centerX / (6.0 * area);
	centerY = centerY / (6.0 * area);

	if (!m_centerPoint)
	{
		m_centerPoint = new GeoPoint();
	}

	m_centerPoint->x = centerX;
	m_centerPoint->y = centerY;
}

ID2D1PathGeometry* SSurface::GetNewD2Geometry(ID2D1Factory1* factory, Scaler* scaler)
{
	if (m_numPoints > 2)
	{
		ID2D1PathGeometry* newGeometry = nullptr;
		auto points = new D2D1_POINT_2F[m_numPoints];

		 //memory leak(little bit)
		for (int i = 0; i < m_numPoints; i++)
		{
			scaler->WorldToDevice_F(m_pPoints[i].x, m_pPoints[i].y, &points[i].x, &points[i].y);
		}

		if (m_numParts == 1)
		{
			//memory leak
			auto result = factory->CreatePathGeometry((ID2D1PathGeometry **)&newGeometry);
			if (SUCCEEDED(result))
			{
				ID2D1PathGeometry *pPathGeometry = (ID2D1PathGeometry *)newGeometry;
				ID2D1GeometrySink *pSink = nullptr;
				result = pPathGeometry->Open(&pSink);
				if (SUCCEEDED(result))
				{
					pSink->BeginFigure(points[0], D2D1_FIGURE_BEGIN_FILLED);
					pSink->AddLines(&points[1], m_numPoints - 1);
					pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
					result = pSink->Close();
					if (SUCCEEDED(result))
					{
						SafeRelease(&pSink);
					}
				}
			}
		}
		else if (m_numParts > 1)
		{
			// Area
			ID2D1PathGeometry **segGeometry = new ID2D1PathGeometry*[m_numParts];
			for (int i = 0; i < m_numParts; i++)
			{
				//memory leak
				auto result = factory->CreatePathGeometry(&segGeometry[i]);
				if (SUCCEEDED(result))
				{
					ID2D1GeometrySink *pSink = nullptr;
					result = segGeometry[i]->Open(&pSink);
					if (SUCCEEDED(result))
					{
						pSink->BeginFigure(points[m_pParts[i]], D2D1_FIGURE_BEGIN_FILLED);
						pSink->AddLines(&points[m_pParts[i] + 1], GetNumPointPerPart(i) - 1);
						pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
						result = pSink->Close();
						if (SUCCEEDED(result))
						{
							SafeRelease(&pSink);
						}
					}

				}
			}

			auto result = factory->CreateGeometryGroup(
				D2D1_FILL_MODE_ALTERNATE,
				(ID2D1Geometry **)segGeometry,
				m_numParts,
				(ID2D1GeometryGroup **)&(newGeometry));

			for (int i = 0; i < m_numParts; i++)
			{
				SafeRelease(&segGeometry[i]);
			}

			delete[] segGeometry;
			segGeometry = nullptr;
	     }

		delete[] points;
		points = nullptr;

		return newGeometry;

	}
	return nullptr;
}