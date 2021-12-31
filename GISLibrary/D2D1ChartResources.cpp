#include "stdafx.h"
#include "D2D1ChartResources.h"
#include "GISLibrary.h"

namespace GISLibrary
{
	D2D1ChartResources::D2D1ChartResources()
	{
		m_pOwnShipGuardZone = nullptr;
		m_pRouteBufferZone = nullptr;
		m_pResources = nullptr;
	}

	D2D1ChartResources::~D2D1ChartResources()
	{
		if (m_pOwnShipGuardZone)
			SafeRelease(&m_pOwnShipGuardZone);
		if (m_pRouteBufferZone)
			SafeRelease(&m_pRouteBufferZone);
	}

	void D2D1ChartResources::SetChartResources(D2D1Resources* _resources)
	{
		m_pResources = _resources;
	}

	ID2D1PathGeometry* D2D1ChartResources::GetGuardZoneGeometry()
	{
		return m_pOwnShipGuardZone;
	}

	ID2D1PathGeometry* D2D1ChartResources::MakeGuardZoneGeometry(D2D1_POINT_2F points[4])
	{
		if (m_pOwnShipGuardZone)
			SafeRelease(&m_pOwnShipGuardZone);

		if (!m_pResources)
			return NULL;

		HRESULT hr = S_OK;
		ID2D1GeometrySink* pGeometrySink = NULL;

		gisLib->WorldToDevice(points[0]);
		gisLib->WorldToDevice(points[1]);
		gisLib->WorldToDevice(points[2]);
		gisLib->WorldToDevice(points[3]);

		hr = m_pResources->pD2Factory->CreatePathGeometry(&m_pOwnShipGuardZone);
		if (SUCCEEDED(hr))
		{
			hr = m_pOwnShipGuardZone->Open(&pGeometrySink);

			pGeometrySink->SetFillMode(D2D1_FILL_MODE_WINDING);

			pGeometrySink->BeginFigure(
				D2D1::Point2F(points[3].x, points[3].y),
				D2D1_FIGURE_BEGIN_FILLED
			);

			pGeometrySink->AddLines(points, 4);
			pGeometrySink->EndFigure(D2D1_FIGURE_END_CLOSED);
		}
		hr = pGeometrySink->Close();
		SafeRelease(&pGeometrySink);

		return m_pOwnShipGuardZone;
	}

	ID2D1PathGeometry* D2D1ChartResources::GetRouteBufferZoneGeometry()
	{
		return m_pRouteBufferZone;
	}

	void D2D1ChartResources::SetRouteBufferZoneGeometry(ID2D1PathGeometry* _geo)
	{
		if (m_pRouteBufferZone)
			SafeRelease(&m_pRouteBufferZone);
		m_pRouteBufferZone = _geo;
	}

	void D2D1ChartResources::SetGuardZone(D2D1_POINT_2F points[4])
	{
		this->guardZone[0] = points[0];
		this->guardZone[1] = points[1];
		this->guardZone[2] = points[2];
		this->guardZone[3] = points[3];
	}

	D2D1_POINT_2F* D2D1ChartResources::GetGuardZone()
	{
		return guardZone;
	}
}