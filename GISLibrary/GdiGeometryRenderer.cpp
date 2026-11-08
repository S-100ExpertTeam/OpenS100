#include "stdafx.h"
#include "GdiGeometryRenderer.h"
#include "../GeoMetryLibrary/Geometry.h"
#include "../GeoMetryLibrary/GeoPoint.h"
#include "../GeoMetryLibrary/GeoPointZ.h"
#include "../GeoMetryLibrary/GeoMultiPoint.h"
#include "../GeoMetryLibrary/GeoMultiPointZ.h"
#include "../GeoMetryLibrary/GeoPolygon.h"
#include "../GeoMetryLibrary/GeoPolyline.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"
#include "../GeoMetryLibrary/Scaler.h"

#include <sstream>
#include <string>
#include <vector>

namespace
{
	POINT ToDevice(Scaler& scaler, double x, double y)
	{
		POINT p{};
		scaler.WorldToDevice(x, y, &p.x, &p.y);
		return p;
	}

	void DrawPoint(GeoPoint& g, HDC& hDC, Scaler& scaler, double offset)
	{
		const POINT p = ToDevice(scaler, g.x + offset, g.y);
		::Ellipse(hDC, p.x - 1, p.y - 1, p.x + 1, p.y + 1);
	}

	void DrawPointZ(GeoPointZ& g, HDC& hDC, Scaler& scaler, double offset)
	{
		const POINT p = ToDevice(scaler, g.x + offset, g.y);
		std::wstringstream stream;
		stream << g.z;
		const std::wstring text = stream.str();
		::TextOutW(hDC, p.x - 15, p.y - 30, text.c_str(), static_cast<int>(text.size()));
	}

	void FillSquares(HDC& hDC, Scaler& scaler, double offset, int count, const GeoPoint* points, COLORREF color)
	{
		HBRUSH brush = ::CreateSolidBrush(color);
		for (int i = 0; i < count; i++)
		{
			const POINT p = ToDevice(scaler, points[i].x + offset, points[i].y);
			const RECT r{ p.x - 2, p.y - 2, p.x + 2, p.y + 2 };
			::FillRect(hDC, &r, brush);
		}
		::DeleteObject(brush);
	}

	void DrawPolygon(GeoPolygon& g, HDC& hDC, Scaler& scaler, double offset)
	{
		std::vector<POINT> device(static_cast<size_t>(g.NumPoints));
		for (int i = 0; i < g.NumPoints; i++)
		{
			device[i] = ToDevice(scaler, g.Points[i].x + offset, g.Points[i].y);
		}

		HBRUSH brush = ::CreateSolidBrush(RGB(255, 255, 255));
		HPEN pen = ::CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		HGDIOBJ oldPen = ::SelectObject(hDC, pen);
		HGDIOBJ oldBrush = ::SelectObject(hDC, brush);

		for (int part = 0; part < g.NumParts; part++)
		{
			const int start = g.Parts[part];
			const int end = (part == g.NumParts - 1) ? g.NumPoints : g.Parts[part + 1];
			if (end > start)
			{
				::Polygon(hDC, &device[start], end - start);
			}
		}

		::SelectObject(hDC, oldBrush);
		::SelectObject(hDC, oldPen);
		::DeleteObject(brush);
		::DeleteObject(pen);
	}

	// Polyline with coarse clipping against the visible map extent, part by part.
	void DrawPolyline(GeoPolyline& g, HDC& hDC, Scaler& scaler, double offset)
	{
		std::vector<POINT> device(static_cast<size_t>(g.NumPoints));
		MBR visible;
		visible.xmin = scaler.mxMin - 0.2;
		visible.xmax = scaler.mxMax + 0.2;
		visible.ymin = scaler.myMin - 0.2;
		visible.ymax = scaler.myMax + 0.2;

		int viewPointNum = 0;
		int partsIndex = 1;
		int lastPointIndex = -1;
		unsigned preLoc = static_cast<unsigned>(-1);

		for (int i = 0; i < g.NumPoints; i++)
		{
			const unsigned curLoc = getClippingLocation(&visible, g.Points[i].x + offset, g.Points[i].y);
			if (curLoc == 0 || curLoc != preLoc)
			{
				if (preLoc != static_cast<unsigned>(-1) && preLoc != 0 && lastPointIndex != i - 1)
				{
					device[viewPointNum++] = ToDevice(scaler, g.Points[i - 1].x + offset, g.Points[i - 1].y);
				}
				device[viewPointNum++] = ToDevice(scaler, g.Points[i].x + offset, g.Points[i].y);
				lastPointIndex = i;
			}
			preLoc = curLoc;

			const bool partEnds = (partsIndex < g.NumParts && i == g.Parts[partsIndex] - 1) || i == g.NumPoints - 1;
			if (partEnds)
			{
				if (viewPointNum > 0)
				{
					::Polyline(hDC, device.data(), viewPointNum);
				}
				preLoc = static_cast<unsigned>(-1);
				++partsIndex;
				viewPointNum = 0;
			}
		}
	}
}

namespace GISLibrary
{
	namespace GdiRender
	{
		void Draw(Geometry& geometry, HDC& hDC, Scaler* scaler, double offset)
		{
			if (!scaler)
			{
				return;
			}
			// Most-derived types first.
			if (auto pz = dynamic_cast<GeoPointZ*>(&geometry))
			{
				DrawPointZ(*pz, hDC, *scaler, offset);
			}
			else if (auto p = dynamic_cast<GeoPoint*>(&geometry))
			{
				DrawPoint(*p, hDC, *scaler, offset);
			}
			else if (auto mz = dynamic_cast<GeoMultiPointZ*>(&geometry))
			{
				FillSquares(hDC, *scaler, offset, mz->m_numPoints, mz->m_pPoints, RGB(165, 191, 221));
			}
			else if (auto m = dynamic_cast<GeoMultiPoint*>(&geometry))
			{
				FillSquares(hDC, *scaler, offset, m->m_numPoints, m->m_pPoints, RGB(255, 191, 221));
			}
			else if (auto poly = dynamic_cast<GeoPolygon*>(&geometry))
			{
				DrawPolygon(*poly, hDC, *scaler, offset);
			}
			else if (auto line = dynamic_cast<GeoPolyline*>(&geometry))
			{
				DrawPolyline(*line, hDC, *scaler, offset);
			}
		}
	}
}
