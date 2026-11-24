#include "stdafx.h"
#include "S100EditRender.h"
#include "GISLibrary.h"
#include "FeatureHighlight.h"
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

S100EditRender::S100EditRender(S101Cell* enc, Scaler* scaler)
{
	this->enc = enc;
	this->scaler = scaler;
}

S100EditRender::~S100EditRender()
{

}

void S100EditRender::Set(S101Cell* _enc, Scaler* _scaler, R_FeatureRecord* _feature, s100::render::IRenderer* _renderer)
{
	this->renderer = _renderer;
	this->enc = _enc;
	this->scaler = _scaler;
	this->feature = _feature;

	if (_enc == nullptr && _feature == nullptr)
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
	if (!enc || !scaler || !geom || !renderer)
	{
		return;
	}
	long sx = 0;
	long sy = 0;
	scaler->WorldToDevice(geom->x, geom->y, &sx, &sy);
	GISLibrary::DrawVertexHandle(*renderer, static_cast<float>(sx), static_cast<float>(sy), pointIndex == 0 && partIndex == 0);
}

void S100EditRender::ShowPoint(SMultiPoint* geom)
{
	if (!enc || !scaler || !geom || !renderer)
	{
		return;
	}
	for (int i = 0; i < geom->GetNumPoints(); i++)
	{
		long sx = 0;
		long sy = 0;
		scaler->WorldToDevice(geom->GetX(i), geom->GetY(i), &sx, &sy);
		GISLibrary::DrawVertexHandle(*renderer, static_cast<float>(sx), static_cast<float>(sy), pointIndex == i && partIndex == 0);
	}
}

// The vertices of a curve; the selected one is drawn last, over its neighbours.
void S100EditRender::ShowPoint(SCurve* geom)
{
	if (!enc || !scaler || !geom || !renderer)
	{
		return;
	}
	long selectedSX = 0;
	long selectedSY = 0;
	for (int i = 0; i < geom->GetNumPoints(); i++)
	{
		long sx = 0;
		long sy = 0;
		scaler->WorldToDevice(geom->GetX(i), geom->GetY(i), &sx, &sy);
		GISLibrary::DrawVertexHandle(*renderer, static_cast<float>(sx), static_cast<float>(sy), false);
		if (pointIndex == i && partIndex == 0)
		{
			selectedSX = sx;
			selectedSY = sy;
		}
	}
	if (pointIndex >= 0 && partIndex == 0)
	{
		GISLibrary::DrawVertexHandle(*renderer, static_cast<float>(selectedSX), static_cast<float>(selectedSY), true);
	}
}

void S100EditRender::ShowPoint(SCompositeCurve* geom)
{
	if (!enc || !scaler || !geom || !renderer)
	{
		return;
	}
	long selectedSX = 0;
	long selectedSY = 0;
	for (int i = 0; i < geom->GetPointCount(); i++)
	{
		const auto mp = geom->GetXY(i);
		long sx = 0;
		long sy = 0;
		scaler->WorldToDevice(mp.x, mp.y, &sx, &sy);
		GISLibrary::DrawVertexHandle(*renderer, static_cast<float>(sx), static_cast<float>(sy), false);
		if (pointIndex == i && partIndex == 0)
		{
			selectedSX = sx;
			selectedSY = sy;
		}
	}
	if (pointIndex >= 0 && partIndex == 0)
	{
		GISLibrary::DrawVertexHandle(*renderer, static_cast<float>(selectedSX), static_cast<float>(selectedSY), true);
	}
}

void S100EditRender::ShowPoint(SSurface* geom)
{
	if (!enc || !scaler || !geom || !renderer)
	{
		return;
	}
	long selectedSX = 0;
	long selectedSY = 0;
	for (int i = 0; i < geom->GetNumPart(); i++)
	{
		for (int j = 0; j < geom->GetNumPointPerPart(i); j++)
		{
			const auto mp = geom->GetXY(i, j);
			long sx = 0;
			long sy = 0;
			scaler->WorldToDevice(mp.x, mp.y, &sx, &sy);
			GISLibrary::DrawVertexHandle(*renderer, static_cast<float>(sx), static_cast<float>(sy), false);
			if (pointIndex == j && partIndex == i)
			{
				selectedSX = sx;
				selectedSY = sy;
			}
		}
	}
	if (pointIndex >= 0 && partIndex >= 0)
	{
		GISLibrary::DrawVertexHandle(*renderer, static_cast<float>(selectedSX), static_cast<float>(selectedSY), true);
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

void S100EditRender::SelectByScreen(int /*sx*/, int /*sy*/, SPoint* /*geom*/)
{
	pointIndex = 0;
	partIndex = 0;
}

void S100EditRender::SelectByScreen(int sx, int sy, SMultiPoint* geom)
{
	if (scaler && geom)
	{
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
	if (scaler && geom)
	{
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
	if (scaler && geom)
	{
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
	if (scaler && geom)
	{
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

double S100EditRender::GetSimpleDistance(double x1, double y1, double x2, double y2)
{
	return pow(x1 - x2, 2) + pow(y1 - y2, 2);
}