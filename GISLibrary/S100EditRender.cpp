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

void S100EditRender::ShowPoint()
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
			ShowPoint(point);
		}
		else if (115 == rcnm)
		{
			auto multiPoint = (SMultiPoint*)feature->GetGeometry();
			ShowPoint(multiPoint);
		}
		else if (120 == rcnm)
		{
			auto curve = (SCurve*)feature->GetGeometry();
			ShowPoint(curve);
		}
		else if (125 == rcnm)
		{
			auto compositeCurve = (SCompositeCurve*)feature->GetGeometry();
			ShowPoint(compositeCurve);
		}
		else if (130 == rcnm)
		{
			auto surface = (SSurface*)feature->GetGeometry();
			ShowPoint(surface);
		}
	}
}

void S100EditRender::ShowPoint(SPoint* geom)
{
	if (gisLib->D2.pRT && geom)
	{
		auto scaler = gisLib->GetScaler();

		D2D1_RECT_F rect = { -5, -5, 5, 5 };

		auto d2 = gisLib->D2;

		d2.pBrush->SetOpacity(1);

		d2.pRT->SetTransform(D2D1::Matrix3x2F::Identity());

		auto mx = geom->x;
		auto my = geom->y;
		long sx = 0;
		long sy = 0;

		scaler->WorldToDevice(mx, my, &sx, &sy);

		d2.pRT->SetTransform(D2D1::Matrix3x2F::Translation(sx, sy));

		d2.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
		d2.pRT->FillRectangle(rect, d2.pBrush);

		if (pointIndex == 0 && partIndex == 0)
		{
			d2.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
			d2.pRT->DrawRectangle(rect, d2.pBrush, 3, d2.SolidStrokeStyle());
		}
		else
		{
			d2.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			d2.pRT->DrawRectangle(rect, d2.pBrush, 1, d2.SolidStrokeStyle());
		}
	}
}

void S100EditRender::ShowPoint(SMultiPoint* geom)
{
	if (gisLib->D2.pRT && geom)
	{
		auto scaler = gisLib->GetScaler();

		D2D1_RECT_F rect = { -5, -5, 5, 5 };

		auto d2 = gisLib->D2;

		d2.pBrush->SetOpacity(1);

		d2.pRT->SetTransform(D2D1::Matrix3x2F::Identity());

		auto numPoint = geom->GetNumPoints();
		for (int i = 0; i < numPoint; i++)
		{
			auto mx = geom->GetX(i);
			auto my = geom->GetY(i);
			long sx = 0;
			long sy = 0;

			scaler->WorldToDevice(mx, my, &sx, &sy);

			d2.pRT->SetTransform(D2D1::Matrix3x2F::Translation(sx, sy));

			d2.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
			d2.pRT->FillRectangle(rect, d2.pBrush);

			if (pointIndex == i && partIndex == 0)
			{
				d2.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
				d2.pRT->DrawRectangle(rect, d2.pBrush, 3, d2.SolidStrokeStyle());
			}
			else
			{
				d2.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
				d2.pRT->DrawRectangle(rect, d2.pBrush, 1, d2.SolidStrokeStyle());
			}
		}
	}
}

void S100EditRender::ShowPoint(SCurve* geom)
{
	if (gisLib->D2.pRT && geom)
	{
		long selectedSX = 0;
		long selectedSY = 0;

		auto scaler = gisLib->GetScaler();

		D2D1_RECT_F rect = { -5, -5, 5, 5 };

		auto d2 = gisLib->D2;

		d2.pBrush->SetOpacity(1);

		d2.pRT->SetTransform(D2D1::Matrix3x2F::Identity());

		auto numPoint = geom->GetNumPoints();
		for (int i = 0; i < numPoint; i++)
		{
			auto mx = geom->GetX(i);
			auto my = geom->GetY(i);
			long sx = 0;
			long sy = 0;

			scaler->WorldToDevice(mx, my, &sx, &sy);

			d2.pRT->SetTransform(D2D1::Matrix3x2F::Translation(sx, sy));

			d2.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
			d2.pRT->FillRectangle(rect, d2.pBrush);

			if (pointIndex == i && partIndex == 0)
			{
				selectedSX = sx;
				selectedSY = sy;
			}
		
			d2.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			d2.pRT->DrawRectangle(rect, d2.pBrush, 1, d2.SolidStrokeStyle());
		}

		if (pointIndex >= 0 && partIndex == 0)
		{
			d2.pRT->SetTransform(D2D1::Matrix3x2F::Translation(selectedSX, selectedSY));

			d2.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
			d2.pRT->FillRectangle(rect, d2.pBrush);

			d2.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
			d2.pRT->DrawRectangle(rect, d2.pBrush, 2, d2.SolidStrokeStyle());
		}
	}
}

void S100EditRender::ShowPoint(SCompositeCurve* geom)
{
	if (gisLib->D2.pRT && geom)
	{
		long selectedSX = 0;
		long selectedSY = 0;

		auto scaler = gisLib->GetScaler();

		D2D1_RECT_F rect = { -5, -5, 5, 5 };

		auto d2 = gisLib->D2;

		d2.pBrush->SetOpacity(1);

		d2.pRT->SetTransform(D2D1::Matrix3x2F::Identity());

		auto numPoint = geom->GetPointCount();
		for (int i = 0; i < numPoint; i++)
		{
			auto mp = geom->GetXY(i);
			long sx = 0;
			long sy = 0;

			scaler->WorldToDevice(mp.x, mp.y, &sx, &sy);

			d2.pRT->SetTransform(D2D1::Matrix3x2F::Translation(sx, sy));

			d2.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
			d2.pRT->FillRectangle(rect, d2.pBrush);

			if (pointIndex == i && partIndex == 0)
			{
				selectedSX = sx;
				selectedSY = sy;
			}
	
			d2.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
			d2.pRT->DrawRectangle(rect, d2.pBrush, 1, d2.SolidStrokeStyle());
		}

		if (pointIndex >= 0 && partIndex == 0)
		{
			d2.pRT->SetTransform(D2D1::Matrix3x2F::Translation(selectedSX, selectedSY));

			d2.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
			d2.pRT->FillRectangle(rect, d2.pBrush);

			d2.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
			d2.pRT->DrawRectangle(rect, d2.pBrush, 2, d2.SolidStrokeStyle());
		}
	}
}

void S100EditRender::ShowPoint(SSurface* geom)
{
	if (gisLib->D2.pRT && geom)
	{
		long selectedSX = 0;
		long selectedSY = 0;

		auto scaler = gisLib->GetScaler();

		D2D1_RECT_F rect = { -5, -5, 5, 5 };

		auto d2 = gisLib->D2;

		d2.pBrush->SetOpacity(1);

		d2.pRT->SetTransform(D2D1::Matrix3x2F::Identity());

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

				d2.pRT->SetTransform(D2D1::Matrix3x2F::Translation(sx, sy));

				d2.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
				d2.pRT->FillRectangle(rect, d2.pBrush);

				if (pointIndex == j && partIndex == i)
				{
					selectedSX = sx;
					selectedSY = sy;
				}
				else
				{
					d2.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
					d2.pRT->DrawRectangle(rect, d2.pBrush, 1, d2.SolidStrokeStyle());
				}
			}
		}

		if (pointIndex >= 0 && partIndex >= 0)
		{
			d2.pRT->SetTransform(D2D1::Matrix3x2F::Translation(selectedSX, selectedSY));

			d2.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
			d2.pRT->FillRectangle(rect, d2.pBrush);

			d2.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
			d2.pRT->DrawRectangle(rect, d2.pBrush, 2, d2.SolidStrokeStyle());
		}
	}
}

void S100EditRender::SelectByScreen(int sx, int sy)
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
			SelectByScreen(sx, sy, point);
		}
		else if (115 == rcnm)
		{
			auto multiPoint = (SMultiPoint*)feature->GetGeometry();
			SelectByScreen(sx, sy, multiPoint);
		}
		else if (120 == rcnm)
		{
			auto curve = (SCurve*)feature->GetGeometry();
			SelectByScreen(sx, sy, curve);
		}
		else if (125 == rcnm)
		{
			auto compositeCurve = (SCompositeCurve*)feature->GetGeometry();
			SelectByScreen(sx, sy, compositeCurve);
		}
		else if (130 == rcnm)
		{
			auto surface = (SSurface*)feature->GetGeometry();
			SelectByScreen(sx, sy, surface);
		}
	}
}

void S100EditRender::SelectByScreen(int sx, int sy, SPoint* geom)
{
	pointIndex = 0;
	partIndex = 0;
}

void S100EditRender::SelectByScreen(int sx, int sy, SMultiPoint* geom)
{
	if (geom)
	{
		auto scaler = gisLib->GetScaler();

		double mx = 0;
		double my = 0;

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

void S100EditRender::SelectByScreen(int sx, int sy, SCurve* geom)
{
	if (geom)
	{
		auto scaler = gisLib->GetScaler();

		double mx = 0;
		double my = 0;

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

void S100EditRender::SelectByScreen(int sx, int sy, SCompositeCurve* geom)
{
	if (geom)
	{
		auto scaler = gisLib->GetScaler();

		double mx = 0;
		double my = 0;

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

void S100EditRender::SelectByScreen(int sx, int sy, SSurface* geom)
{
	if (geom)
	{
		auto scaler = gisLib->GetScaler();

		double mx = 0;
		double my = 0;

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

void S100EditRender::UpdatePoint(int sx, int sy)
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
			UpdatePoint(sx, sy, point);
			point->CreateD2Geometry(gisLib->D2.pD2Factory);
		}
		else if (115 == rcnm)
		{
			auto multiPoint = (SMultiPoint*)feature->GetGeometry();
			UpdatePoint(sx, sy, multiPoint);
			multiPoint->CreateD2Geometry(gisLib->D2.pD2Factory);
		}
		else if (120 == rcnm)
		{
			auto curve = (SCurve*)feature->GetGeometry();
			UpdatePoint(sx, sy, curve);
			curve->CreateD2Geometry(gisLib->D2.pD2Factory);
		}
		else if (125 == rcnm)
		{
			auto compositeCurve = (SCompositeCurve*)feature->GetGeometry();
			UpdatePoint(sx, sy, compositeCurve);
			compositeCurve->CreateD2Geometry(gisLib->D2.pD2Factory);
		}
		else if (130 == rcnm)
		{
			auto surface = (SSurface*)feature->GetGeometry();
			UpdatePoint(sx, sy, surface);
			surface->CreateD2Geometry(gisLib->D2.pD2Factory);
		}
	}
}

void S100EditRender::UpdatePoint(int sx, int sy, SPoint* geom)
{
	if (geom)
	{
		auto scaler = gisLib->GetScaler();
		double mx = 0;
		double my = 0;

		scaler->DeviceToWorld(sx, sy, &mx, &my);
		geom->SetPoint(mx, my);
		geom->SetMBR();

		unsigned char* wkb = nullptr;
		int wkbSize = 0;
		geom->ExportToWkb(&wkb, &wkbSize);

		gisLib->creator.fc = gisLib->catalogManager.getFC("S-101");
		gisLib->creator.enc = enc;
		gisLib->creator.SetPointGeometry(feature, wkb, wkbSize);

		delete[] wkb;
		wkb = nullptr;

		gisLib->S101RebuildPortrayal();
	}
}

void S100EditRender::UpdatePoint(int sx, int sy, SMultiPoint* geom)
{
	if (geom)
	{
		auto scaler = gisLib->GetScaler();
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

		gisLib->creator.fc = gisLib->catalogManager.getFC("S-101");
		gisLib->creator.enc = enc;
		gisLib->creator.SetMultiPointGeometry(feature, wkb, wkbSize);

		delete[] wkb;
		wkb = nullptr;

		gisLib->S101RebuildPortrayal();
	}
}

void S100EditRender::UpdatePoint(int sx, int sy, SCurve* geom)
{
	if (geom)
	{
		auto scaler = gisLib->GetScaler();
		double mx = 0;
		double my = 0;

		scaler->DeviceToWorld(sx, sy, &mx, &my);

		geom->Set(pointIndex, mx, my);
		geom->SetMBR();

		unsigned char* wkb = nullptr;
		int wkbSize = 0;
		geom->ExportToWkb(&wkb, &wkbSize);

		gisLib->creator.fc = gisLib->catalogManager.getFC("S-101");
		gisLib->creator.enc = enc;
		gisLib->creator.SetCurveGeometry(feature, wkb, wkbSize);

		delete[] wkb;
		wkb = nullptr;

		gisLib->S101RebuildPortrayal();
	}
}

void S100EditRender::UpdatePoint(int sx, int sy, SCompositeCurve* geom)
{
	if (geom)
	{
		auto scaler = gisLib->GetScaler();
		double mx = 0;
		double my = 0;

		scaler->DeviceToWorld(sx, sy, &mx, &my);

		geom->Set(pointIndex, mx, my);
		geom->SetMBR();

		unsigned char* wkb = nullptr;
		int wkbSize = 0;
		geom->ExportToWkb(&wkb, &wkbSize);

		gisLib->creator.fc = gisLib->catalogManager.getFC("S-101");
		gisLib->creator.enc = enc;
		gisLib->creator.SetCompositeCurveGeometry(feature, wkb, wkbSize);

		delete[] wkb;
		wkb = nullptr;

		gisLib->S101RebuildPortrayal();
	}
}

void S100EditRender::UpdatePoint(int sx, int sy, SSurface* geom)
{
	if (geom)
	{
		auto scaler = gisLib->GetScaler();
		double mx = 0;
		double my = 0;

		scaler->DeviceToWorld(sx, sy, &mx, &my);

		geom->SetXY(partIndex, pointIndex, mx, my);
		geom->SetMBR();

		unsigned char* wkb = nullptr;
		int wkbSize = 0;
		geom->ExportToWkb(&wkb, &wkbSize);

		gisLib->creator.fc = gisLib->catalogManager.getFC("S-101");
		gisLib->creator.enc = enc;
		gisLib->creator.SetSurfaceGeometry(feature, wkb, wkbSize);

		delete[] wkb;
		wkb = nullptr;

		gisLib->S101RebuildPortrayal();
	}
}

double S100EditRender::GetSimpleDistance(double x1, double y1, double x2, double y2)
{
	return pow(x1 - x2, 2) + pow(y1 - y2, 2);
}