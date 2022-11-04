#include "stdafx.h"
#include "BackgroundLayer.h"
#include "BackgroundSHPFile.h"

BackgroundLayer::BackgroundLayer()
{

}

BackgroundLayer::~BackgroundLayer()
{
	SafeRelease(&geometryGroup);
}

void BackgroundLayer::CreateGeometry(ID2D1Factory1* pD2Factory, BackgroundSHPFile* shp)
{
	HRESULT hr = S_OK;
	std::vector<ID2D1PathGeometry1*> vecGeometry;

	for (int i = 0; i < shp->GetRecordCount(); i++)
	{
		SHPPolygon* polygon = shp->GetPolygon(i);
		int pointIndex = 0;
		if (!polygon)
			continue;


		for (int j = 0; j < polygon->GetNumParts(); j++)
		{
			ID2D1PathGeometry1* geometry = nullptr;
			hr = pD2Factory->CreatePathGeometry(&geometry);

			if (!SUCCEEDED(hr))
			{
				continue;
			}

			ID2D1GeometrySink* sink = nullptr;
			hr = geometry->Open(&sink);

			if (!SUCCEEDED(hr))
			{
				SafeRelease(&geometry);
				continue;
			}

			D2D1_POINT_2F startPoint;
			startPoint.x = polygon->GetX(pointIndex);
			startPoint.y = -polygon->GetY(pointIndex);
			pointIndex++;
			sink->BeginFigure(ProjectionAndApplyFactor(startPoint), D2D1_FIGURE_BEGIN_FILLED);
			
			for (int k = 1; k < polygon->GetNumPointsPerPart(j); k++)
			{
				D2D1_POINT_2F point;
				point.x = polygon->GetX(pointIndex);
				point.y = -polygon->GetY(pointIndex);
				pointIndex++;
				sink->AddLine(ProjectionAndApplyFactor(point));
			}

			sink->EndFigure(D2D1_FIGURE_END_CLOSED);

			sink->Close();
			SafeRelease(&sink);

			vecGeometry.push_back(geometry);
		}
	}

	hr = pD2Factory->CreateGeometryGroup(
		D2D1_FILL_MODE_ALTERNATE,
		(ID2D1Geometry**)&vecGeometry.front(),
		(UINT32)vecGeometry.size(),
		(ID2D1GeometryGroup**)&geometryGroup);

	if (!SUCCEEDED(hr))
	{
		//OutputDebugString(_T("Faield to create backgound layer geometry\n"));
	}
}

void BackgroundLayer::Draw(ID2D1DCRenderTarget* pRenderTarget, ID2D1SolidColorBrush* pBrush)
{
	if (geometryGroup)
	{
		pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::DarkGoldenrod));
		pRenderTarget->FillGeometry(geometryGroup, pBrush);
	}
}