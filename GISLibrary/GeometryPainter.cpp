#include "stdafx.h"
#include "GeometryPainter.h"

#include "../GeoMetryLibrary/Geometry.h"
#include "../GeoMetryLibrary/GeoPoint.h"
#include "../GeoMetryLibrary/GeoPointZ.h"
#include "../GeoMetryLibrary/GeoMultiPoint.h"
#include "../GeoMetryLibrary/GeoMultiPointZ.h"
#include "../GeoMetryLibrary/GeoPolygon.h"
#include "../GeoMetryLibrary/GeoPolyline.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"
#include "../GeoMetryLibrary/Scaler.h"

#include <format>
#include <string>
#include <vector>

namespace
{
	using s100::render::ColorF;
	using s100::render::IRenderer;
	using s100::render::Path;
	using s100::render::PathFigure;

	const ColorF kBlack{ 0.0f, 0.0f, 0.0f, 1.0f };

	s100::render::PointF ToDevice(Scaler& scaler, double x, double y)
	{
		// Whole pixels, as these shapes have always been placed.
		long sx = 0;
		long sy = 0;
		scaler.WorldToDevice(x, y, &sx, &sy);
		return { static_cast<float>(sx), static_cast<float>(sy) };
	}

	s100::render::Stroke Thin()
	{
		s100::render::Stroke stroke;
		stroke.width = 1.0f;
		return stroke;
	}

	void FillSquare(IRenderer& renderer, s100::render::PointF at, float half, const ColorF& color)
	{
		PathFigure square;
		square.start = { at.x - half, at.y - half };
		square.lines = { { at.x + half, at.y - half }, { at.x + half, at.y + half }, { at.x - half, at.y + half } };
		square.closed = true;
		Path path;
		path.figures.push_back(std::move(square));
		s100::render::Fill fill;
		fill.color = color;
		renderer.FillPath(path, fill);
	}

	// Every part filled on its own (a part inside another is not a hole), then all the edges.
	void DrawPolygon(GeoPolygon& g, IRenderer& renderer, Scaler& scaler, double offset)
	{
		Path edges;
		for (int part = 0; part < g.NumParts; part++)
		{
			const int start = g.Parts[part];
			const int end = (part == g.NumParts - 1) ? g.NumPoints : g.Parts[part + 1];
			if (end - start < 2)
			{
				continue;
			}
			PathFigure ring;
			ring.start = ToDevice(scaler, g.Points[start].x + offset, g.Points[start].y);
			for (int i = start + 1; i < end; i++)
			{
				ring.lines.push_back(ToDevice(scaler, g.Points[i].x + offset, g.Points[i].y));
			}
			ring.closed = true;

			Path inside;
			inside.figures.push_back(ring);
			s100::render::Fill white;
			white.color = { 1.0f, 1.0f, 1.0f, 1.0f };
			renderer.FillPath(inside, white);
			edges.figures.push_back(std::move(ring));
		}
		if (!edges.figures.empty())
		{
			renderer.StrokePath(edges, kBlack, Thin());
		}
	}

	// Polyline with coarse clipping against the visible map extent, part by part.
	void DrawPolyline(GeoPolyline& g, IRenderer& renderer, Scaler& scaler, double offset)
	{
		MBR visible;
		visible.xmin = scaler.mxMin - 0.2;
		visible.xmax = scaler.mxMax + 0.2;
		visible.ymin = scaler.myMin - 0.2;
		visible.ymax = scaler.myMax + 0.2;

		Path path;
		std::vector<s100::render::PointF> device;
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
					device.push_back(ToDevice(scaler, g.Points[i - 1].x + offset, g.Points[i - 1].y));
				}
				device.push_back(ToDevice(scaler, g.Points[i].x + offset, g.Points[i].y));
				lastPointIndex = i;
			}
			preLoc = curLoc;

			const bool partEnds = (partsIndex < g.NumParts && i == g.Parts[partsIndex] - 1) || i == g.NumPoints - 1;
			if (partEnds)
			{
				if (device.size() > 1)
				{
					PathFigure line;
					line.start = device.front();
					line.lines.assign(device.begin() + 1, device.end());
					path.figures.push_back(std::move(line));
				}
				preLoc = static_cast<unsigned>(-1);
				++partsIndex;
				device.clear();
			}
		}
		if (!path.figures.empty())
		{
			renderer.StrokePath(path, kBlack, Thin());
		}
	}
}

namespace GISLibrary
{
	void DrawGeometry(IRenderer& renderer, Geometry& geometry, Scaler& scaler, double offset)
	{
		renderer.SetTransform({});

		// Most-derived types first.
		if (auto pz = dynamic_cast<GeoPointZ*>(&geometry))
		{
			const auto at = ToDevice(scaler, pz->x + offset, pz->y);
			s100::render::TextFont font;
			font.size = 14.0f;
			renderer.FillText(std::format("{}", pz->z), { at.x - 15, at.y - 30 }, font, kBlack);
		}
		else if (auto p = dynamic_cast<GeoPoint*>(&geometry))
		{
			FillSquare(renderer, ToDevice(scaler, p->x + offset, p->y), 1.0f, kBlack);
		}
		else if (auto mz = dynamic_cast<GeoMultiPointZ*>(&geometry))
		{
			for (int i = 0; i < mz->m_numPoints; i++)
			{
				FillSquare(renderer, ToDevice(scaler, mz->m_pPoints[i].x + offset, mz->m_pPoints[i].y), 2.0f,
					{ 165 / 255.0f, 191 / 255.0f, 221 / 255.0f, 1.0f });
			}
		}
		else if (auto m = dynamic_cast<GeoMultiPoint*>(&geometry))
		{
			for (int i = 0; i < m->m_numPoints; i++)
			{
				FillSquare(renderer, ToDevice(scaler, m->m_pPoints[i].x + offset, m->m_pPoints[i].y), 2.0f,
					{ 1.0f, 191 / 255.0f, 221 / 255.0f, 1.0f });
			}
		}
		else if (auto poly = dynamic_cast<GeoPolygon*>(&geometry))
		{
			DrawPolygon(*poly, renderer, scaler, offset);
		}
		else if (auto line = dynamic_cast<GeoPolyline*>(&geometry))
		{
			DrawPolyline(*line, renderer, scaler, offset);
		}
	}
}
