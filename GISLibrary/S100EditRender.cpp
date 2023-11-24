#include "stdafx.h"
#include "S100EditRender.h"
#include "GISLibrary.h"
#include "D2D1Resources.h"
#include "R_FeatureRecord.h"
#include "SPAS.h"
#include "S101Cell.h"

#include "SPoint.h"
#include "SMultiPoint.h"
#include "SCurve.h"
#include "SCompositeCurve.h"
#include "SSurface.h"

S100EditRender::S100EditRender()
{

}

S100EditRender::S100EditRender(S101Cell* enc)
{
	this->enc = enc;
}

S100EditRender::~S100EditRender()
{

} 

void S100EditRender::Set(S101Cell* enc, R_FeatureRecord* feature)
{	
	this->enc = enc;
	this->feature = feature;

	if (enc == nullptr && feature == nullptr)
	{
		pointIndex = -1;
		partIndex = -1;
	}
}

void S100EditRender::ShowPoint(LayerManager* layerManager)
{
	if (nullptr == enc || nullptr == feature)
	{
		return;
	}

	auto spas = feature->GetSPAS();
	if (spas)
	{
		auto rcnm = spas->m_name.RCNM;

		if (110 == rcnm)
		{
			auto point = (SPoint*)feature->GetGeometry();
			ShowPoint(layerManager, point);
		}
		else if (115 == rcnm)
		{
			auto multiPoint = (SMultiPoint*)feature->GetGeometry();
			ShowPoint(layerManager, multiPoint);
		}
		else if (120 == rcnm)
		{
			auto curve = (SCurve*)feature->GetGeometry();
			ShowPoint(layerManager, curve);
		}
		else if (125 == rcnm)
		{
			auto compositeCurve = (SCompositeCurve*)feature->GetGeometry();
			ShowPoint(layerManager, compositeCurve);
		}
		else if (130 == rcnm)
		{
			auto surface = (SSurface*)feature->GetGeometry();
			ShowPoint(layerManager, surface);
		}
	}
}

void S100EditRender::ShowPoint(LayerManager* layerManager, SPoint* geom)
{
	if (!layerManager || !geom)
		return;

	auto d2 = layerManager->GetD2D1Resources();
	auto scaler = layerManager->GetScaler();

	if (d2->pRT)
	{
		D2D1_RECT_F rect = { -5, -5, 5, 5 };

		d2->pBrush->SetOpacity(1);

		d2->pRT->SetTransform(D2D1::Matrix3x2F::Identity());

		auto mx = geom->x;
		auto my = geom->y;
		long sx = 0;
		long sy = 0;

		scaler->WorldToDevice(mx, my, &sx, &sy);

		d2->pRT->SetTransform(D2D1::Matrix3x2F::Translation(sx, sy));

		d2->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
		d2->pRT->FillRectangle(rect, d2->pBrush);

		if (pointIndex == 0 && partIndex == 0)
		{
			d2->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
			d2->pRT->DrawRectangle(rect, d2->pBrush, 3, d2->SolidStrokeStyle());
		}
		else
		{
			d2->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			d2->pRT->DrawRectangle(rect, d2->pBrush, 1, d2->SolidStrokeStyle());
		}
	}
}

void S100EditRender::ShowPoint(LayerManager* layerManager, SMultiPoint* geom)
{
	if (!layerManager || !geom)
		return;

	auto d2 = layerManager->GetD2D1Resources();
	auto scaler = layerManager->GetScaler();

	if (d2->pRT)
	{
		D2D1_RECT_F rect = { -5, -5, 5, 5 };

		d2->pBrush->SetOpacity(1);

		d2->pRT->SetTransform(D2D1::Matrix3x2F::Identity());

		auto numPoint = geom->GetNumPoints();
		for (int i = 0; i < numPoint; i++)
		{
			auto mx = geom->GetX(i);
			auto my = geom->GetY(i);
			long sx = 0;
			long sy = 0;

			scaler->WorldToDevice(mx, my, &sx, &sy);

			d2->pRT->SetTransform(D2D1::Matrix3x2F::Translation(sx, sy));

			d2->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
			d2->pRT->FillRectangle(rect, d2->pBrush);

			if (pointIndex == i && partIndex == 0)
			{
				d2->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
				d2->pRT->DrawRectangle(rect, d2->pBrush, 3, d2->SolidStrokeStyle());
			}
			else
			{
				d2->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
				d2->pRT->DrawRectangle(rect, d2->pBrush, 1, d2->SolidStrokeStyle());
			}
		}
	}
}

void S100EditRender::ShowPoint(LayerManager* layerManager, SCurve* geom)
{
	if (!layerManager || !geom)
		return;

	auto d2 = layerManager->GetD2D1Resources();
	auto scaler = layerManager->GetScaler();

	if (d2->pRT)
	{
		long selectedSX = 0;
		long selectedSY = 0;

		D2D1_RECT_F rect = { -5, -5, 5, 5 };

		d2->pBrush->SetOpacity(1);

		d2->pRT->SetTransform(D2D1::Matrix3x2F::Identity());

		auto numPoint = geom->GetNumPoints();
		for (int i = 0; i < numPoint; i++)
		{
			auto mx = geom->GetX(i);
			auto my = geom->GetY(i);
			long sx = 0;
			long sy = 0;

			scaler->WorldToDevice(mx, my, &sx, &sy);

			d2->pRT->SetTransform(D2D1::Matrix3x2F::Translation(sx, sy));

			d2->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
			d2->pRT->FillRectangle(rect, d2->pBrush);

			if (pointIndex == i && partIndex == 0)
			{
				selectedSX = sx;
				selectedSY = sy;
			}
		
			d2->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			d2->pRT->DrawRectangle(rect, d2->pBrush, 1, d2->SolidStrokeStyle());
		}

		if (pointIndex >= 0 && partIndex == 0)
		{
			d2->pRT->SetTransform(D2D1::Matrix3x2F::Translation(selectedSX, selectedSY));

			d2->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
			d2->pRT->FillRectangle(rect, d2->pBrush);

			d2->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
			d2->pRT->DrawRectangle(rect, d2->pBrush, 2, d2->SolidStrokeStyle());
		}
	}
}

void S100EditRender::ShowPoint(LayerManager* layerManager, SCompositeCurve* geom)
{
	if (!layerManager || !geom)
		return;

	auto d2 = layerManager->GetD2D1Resources();
	auto scaler = layerManager->GetScaler();

	if (d2->pRT)
	{
		long selectedSX = 0;
		long selectedSY = 0;

		D2D1_RECT_F rect = { -5, -5, 5, 5 };

		d2->pBrush->SetOpacity(1);

		d2->pRT->SetTransform(D2D1::Matrix3x2F::Identity());

		auto numPoint = geom->GetPointCount();
		for (int i = 0; i < numPoint; i++)
		{
			auto mp = geom->GetXY(i);
			long sx = 0;
			long sy = 0;

			scaler->WorldToDevice(mp.x, mp.y, &sx, &sy);

			d2->pRT->SetTransform(D2D1::Matrix3x2F::Translation(sx, sy));

			d2->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
			d2->pRT->FillRectangle(rect, d2->pBrush);

			if (pointIndex == i && partIndex == 0)
			{
				selectedSX = sx;
				selectedSY = sy;
			}
	
			d2->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			d2->pRT->DrawRectangle(rect, d2->pBrush, 1, d2->SolidStrokeStyle());
		}

		if (pointIndex >= 0 && partIndex == 0)
		{
			d2->pRT->SetTransform(D2D1::Matrix3x2F::Translation(selectedSX, selectedSY));

			d2->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
			d2->pRT->FillRectangle(rect, d2->pBrush);

			d2->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
			d2->pRT->DrawRectangle(rect, d2->pBrush, 2, d2->SolidStrokeStyle());
		}
	}
}

void S100EditRender::ShowPoint(LayerManager* layerManager, SSurface* geom)
{
	if (!layerManager || !geom)
		return;

	auto d2 = layerManager->GetD2D1Resources();
	auto scaler = layerManager->GetScaler();

	if (d2->pRT)
	{
		long selectedSX = 0;
		long selectedSY = 0;

		D2D1_RECT_F rect = { -5, -5, 5, 5 };

		d2->pBrush->SetOpacity(1);

		d2->pRT->SetTransform(D2D1::Matrix3x2F::Identity());

		auto numPart = geom->GetNumPart();

		for (int i = 0; i < numPart; i++)
		{
			auto numPoint = geom->GetNumPointPerPart(i);
			for (int j = 0; j < numPoint; j++)
			{
				auto mp = geom->GetXY(i, j);
				long sx = 0;
				long sy = 0;

				scaler->WorldToDevice(mp.x, mp.y, &sx, &sy);

				d2->pRT->SetTransform(D2D1::Matrix3x2F::Translation(sx, sy));

				d2->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
				d2->pRT->FillRectangle(rect, d2->pBrush);

				if (pointIndex == j && partIndex == i)
				{
					selectedSX = sx;
					selectedSY = sy;
				}
				else
				{
					d2->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
					d2->pRT->DrawRectangle(rect, d2->pBrush, 1, d2->SolidStrokeStyle());
				}
			}
		}

		if (pointIndex >= 0 && partIndex >= 0)
		{
			d2->pRT->SetTransform(D2D1::Matrix3x2F::Translation(selectedSX, selectedSY));

			d2->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
			d2->pRT->FillRectangle(rect, d2->pBrush);

			d2->pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
			d2->pRT->DrawRectangle(rect, d2->pBrush, 2, d2->SolidStrokeStyle());
		}
	}
}

void S100EditRender::SelectByScreen(int sx, int sy, LayerManager* layerManager)
{
	if (nullptr == enc || nullptr == feature)
	{
		return;
	}

	auto spas = feature->GetSPAS();
	if (spas)
	{
		auto rcnm = spas->m_name.RCNM;

		if (110 == rcnm)
		{
			auto point = (SPoint*)feature->GetGeometry();
			SelectByScreen(sx, sy, layerManager, point);
		}
		else if (115 == rcnm)
		{
			auto multiPoint = (SMultiPoint*)feature->GetGeometry();
			SelectByScreen(sx, sy, layerManager, multiPoint);
		}
		else if (120 == rcnm)
		{
			auto curve = (SCurve*)feature->GetGeometry();
			SelectByScreen(sx, sy, layerManager, curve);
		}
		else if (125 == rcnm)
		{
			auto compositeCurve = (SCompositeCurve*)feature->GetGeometry();
			SelectByScreen(sx, sy, layerManager, compositeCurve);
		}
		else if (130 == rcnm)
		{
			auto surface = (SSurface*)feature->GetGeometry();
			SelectByScreen(sx, sy, layerManager, surface);
		}
	}
}

void S100EditRender::SelectByScreen(int sx, int sy, LayerManager* layerManager, SPoint* geom)
{
	pointIndex = 0;
	partIndex = 0;
}

void S100EditRender::SelectByScreen(int sx, int sy, LayerManager* layerManager, SMultiPoint* geom)
{
	if (layerManager && geom)
	{
		double mx = 0;
		double my = 0;

		auto scaler = layerManager->GetScaler();
		scaler->DeviceToWorld(sx, sy, &mx, &my);

		double shortestDistance = 0;
		int shortestIndex = 0;

		auto numPoint = geom->GetNumPoints();
		for (int i = 0; i < numPoint; i++)
		{
			auto curMX = geom->GetX(i);
			auto curMY = geom->GetY(i);

			auto curDistance = GetSimpleDistance(mx, my, curMX, curMY);

			if (0 == i)
			{
				shortestDistance = curDistance;
				shortestIndex = 0;
			}
			else
			{
				if (curDistance < shortestDistance)
				{
					shortestDistance = curDistance;
					shortestIndex = i;
				}
			}
		}

		pointIndex = shortestIndex;
		partIndex = 0;
	}
}

void S100EditRender::SelectByScreen(int sx, int sy, LayerManager* layerManager, SCurve* geom)
{
	if (layerManager && geom)
	{
		double mx = 0;
		double my = 0;

		auto scaler = layerManager->GetScaler();
		scaler->DeviceToWorld(sx, sy, &mx, &my);

		double shortestDistance = 0;
		int shortestIndex = 0;

		auto numPoint = geom->GetNumPoints();
		for (int i = 0; i < numPoint; i++)
		{
			auto curMX = geom->GetX(i);
			auto curMY = geom->GetY(i);
			
			auto curDistance = GetSimpleDistance(mx, my, curMX, curMY);

			if (0 == i)
			{
				shortestDistance = curDistance;
				shortestIndex = 0;
			}
			else
			{
				if (curDistance < shortestDistance)
				{
					shortestDistance = curDistance;
					shortestIndex = i;
				}
			}
		}

		pointIndex = shortestIndex;
		partIndex = 0;
	}
}

void S100EditRender::SelectByScreen(int sx, int sy, LayerManager* layerManager, SCompositeCurve* geom)
{
	if (layerManager && geom)
	{
		double mx = 0;
		double my = 0;

		auto scaler = layerManager->GetScaler();
		scaler->DeviceToWorld(sx, sy, &mx, &my);

		double shortestDistance = 0;
		int shortestIndex = 0;

		auto numPoint = geom->GetPointCount();
		for (int i = 0; i < numPoint; i++)
		{
			auto curMPoint = geom->GetXY(i);

			auto curDistance = GetSimpleDistance(mx, my, curMPoint.x, curMPoint.y);

			if (0 == i)
			{
				shortestDistance = curDistance;
				shortestIndex = 0;
			}
			else
			{
				if (curDistance < shortestDistance)
				{
					shortestDistance = curDistance;
					shortestIndex = i;
				}
			}
		}

		pointIndex = shortestIndex;
		partIndex = 0;
	}
}

void S100EditRender::SelectByScreen(int sx, int sy, LayerManager* layerManager, SSurface* geom)
{
	if (layerManager && geom)
	{
		double mx = 0;
		double my = 0;

		auto scaler = layerManager->GetScaler();
		scaler->DeviceToWorld(sx, sy, &mx, &my);

		double shortestDistance = 0;
		int shortestIndex = 0;
		int shortestPart = 0;

		auto numPart = geom->GetNumPart();
		for (int i = 0; i < numPart; i++)
		{
			auto numPoint = geom->GetNumPointPerPart(i);
			for (int j = 0; j < numPoint; j++)
			{
				auto curMPoint = geom->GetXY(i, j);

				auto curDistance = GetSimpleDistance(mx, my, curMPoint.x, curMPoint.y);

				if (0 == i && 0 == j)
				{
					shortestDistance = curDistance;
					shortestIndex = j;
					shortestPart = i;
				}
				else
				{
					if (curDistance < shortestDistance)
					{
						shortestDistance = curDistance;
						shortestIndex = j;
						shortestPart = i;
					}
				}
			}
		}


		pointIndex = shortestIndex;
		partIndex = shortestPart;
	}
}

void S100EditRender::UpdatePoint(int sx, int sy, LayerManager* layerManager)
{
	if (nullptr == enc || nullptr == feature || pointIndex < 0 || partIndex < 0)
	{
		return;
	}

	auto spas = feature->GetSPAS();
	if (spas)
	{
		auto rcnm = spas->m_name.RCNM;

		if (110 == rcnm)
		{
			auto point = (SPoint*)feature->GetGeometry();
			UpdatePoint(sx, sy, layerManager, point);
			point->CreateD2Geometry(layerManager->GetD2D1Resources()->pD2Factory);
		}
		else if (115 == rcnm)
		{
			auto multiPoint = (SMultiPoint*)feature->GetGeometry();
			UpdatePoint(sx, sy, layerManager, multiPoint);
			multiPoint->CreateD2Geometry(layerManager->GetD2D1Resources()->pD2Factory);
		}
		else if (120 == rcnm)
		{
			auto curve = (SCurve*)feature->GetGeometry();
			UpdatePoint(sx, sy, layerManager, curve);
			curve->CreateD2Geometry(layerManager->GetD2D1Resources()->pD2Factory);
		}
		else if (125 == rcnm)
		{
			auto compositeCurve = (SCompositeCurve*)feature->GetGeometry();
			UpdatePoint(sx, sy, layerManager, compositeCurve);
			compositeCurve->CreateD2Geometry(layerManager->GetD2D1Resources()->pD2Factory);
		}
		else if (130 == rcnm)
		{
			auto surface = (SSurface*)feature->GetGeometry();
			UpdatePoint(sx, sy, layerManager, surface);
			surface->CreateD2Geometry(layerManager->GetD2D1Resources()->pD2Factory);
		}
	}
}

void S100EditRender::UpdatePoint(int sx, int sy, LayerManager* layerManager, SPoint* geom)
{
	if (layerManager && geom)
	{
		auto scaler = layerManager->GetScaler();
		double mx = 0;
		double my = 0;

		scaler->DeviceToWorld(sx, sy, &mx, &my);
		geom->SetPoint(mx, my);
		geom->SetMBR();

		unsigned char* wkb = nullptr;
		int wkbSize = 0;
		geom->ExportToWkb(&wkb, &wkbSize);

		layerManager->creator->fc = layerManager->catalogManager->getFC("S-101");
		layerManager->creator->enc = enc;
		layerManager->creator->SetPointGeometry(feature, wkb, wkbSize);

		delete[] wkb;
		wkb = nullptr;

		layerManager->S101RebuildPortrayal();
	}
}

void S100EditRender::UpdatePoint(int sx, int sy, LayerManager* layerManager, SMultiPoint* geom)
{
	if (layerManager && geom)
	{
		auto scaler = layerManager->GetScaler();
		double mx = 0;
		double my = 0;
		double mz = 0;

		scaler->DeviceToWorld(sx, sy, &mx, &my);

		mz = geom->GetZ(pointIndex);

		geom->Set(pointIndex, mx, my, mz);
		geom->SetMBR();

		unsigned char* wkb = nullptr;
		int wkbSize = 0;
		geom->ExportToWkb(&wkb, &wkbSize);

		layerManager->creator->fc = layerManager->catalogManager->getFC("S-101");
		layerManager->creator->enc = enc;
		layerManager->creator->SetPointGeometry(feature, wkb, wkbSize);

		delete[] wkb;
		wkb = nullptr;

		layerManager->S101RebuildPortrayal();
	}
}

void S100EditRender::UpdatePoint(int sx, int sy, LayerManager* layerManager, SCurve* geom)
{
	if (layerManager && geom)
	{
		auto scaler = layerManager->GetScaler();
		double mx = 0;
		double my = 0;

		scaler->DeviceToWorld(sx, sy, &mx, &my);

		geom->Set(pointIndex, mx, my);
		geom->SetMBR();

		unsigned char* wkb = nullptr;
		int wkbSize = 0;
		geom->ExportToWkb(&wkb, &wkbSize);

		layerManager->creator->fc = layerManager->catalogManager->getFC("S-101");
		layerManager->creator->enc = enc;
		layerManager->creator->SetPointGeometry(feature, wkb, wkbSize);

		delete[] wkb;
		wkb = nullptr;

		layerManager->S101RebuildPortrayal();
	}
}

void S100EditRender::UpdatePoint(int sx, int sy, LayerManager* layerManager, SCompositeCurve* geom)
{
	if (layerManager && geom)
	{
		auto scaler = layerManager->GetScaler();
		double mx = 0;
		double my = 0;

		scaler->DeviceToWorld(sx, sy, &mx, &my);

		geom->Set(pointIndex, mx, my);
		geom->SetMBR();

		unsigned char* wkb = nullptr;
		int wkbSize = 0;
		geom->ExportToWkb(&wkb, &wkbSize);

		layerManager->creator->fc = layerManager->catalogManager->getFC("S-101");
		layerManager->creator->enc = enc;
		layerManager->creator->SetPointGeometry(feature, wkb, wkbSize);

		delete[] wkb;
		wkb = nullptr;

		layerManager->S101RebuildPortrayal();
	}
}

void S100EditRender::UpdatePoint(int sx, int sy, LayerManager* layerManager, SSurface* geom)
{
	if (layerManager && geom)
	{
		auto scaler = layerManager->GetScaler();
		double mx = 0;
		double my = 0;

		scaler->DeviceToWorld(sx, sy, &mx, &my);

		geom->SetXY(partIndex, pointIndex, mx, my);
		geom->SetMBR();

		unsigned char* wkb = nullptr;
		int wkbSize = 0;
		geom->ExportToWkb(&wkb, &wkbSize);

		layerManager->creator->fc = layerManager->catalogManager->getFC("S-101");
		layerManager->creator->enc = enc;
		layerManager->creator->SetPointGeometry(feature, wkb, wkbSize);

		delete[] wkb;
		wkb = nullptr;

		layerManager->S101RebuildPortrayal();
	}
}

double S100EditRender::GetSimpleDistance(double x1, double y1, double x2, double y2)
{
	return pow(x1 - x2, 2) + pow(y1 - y2, 2);
}