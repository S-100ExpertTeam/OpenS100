#include "stdafx.h"
#include "FeatureHighlight.h"

#include "SCompositeCurve.h"
#include "SCurve.h"
#include "SGeometry.h"
#include "SMultiPoint.h"
#include "SPoint.h"
#include "SSurface.h"

#include "../GeoMetryLibrary/Scaler.h"

namespace
{
	using s100::render::ColorF;
	using s100::render::IRenderer;
	using s100::render::Path;
	using s100::render::PathFigure;
	using s100::render::Stroke;
	using s100::render::TransformStep;

	const ColorF kRed{ 1.0f, 0.0f, 0.0f, 1.0f };

	s100::render::PointF OnScreen(Scaler& scaler, double x, double y)
	{
		s100::render::PointF point;
		scaler.WorldToDevice_F(x, y, &point.x, &point.y);
		return point;
	}

	Stroke Thick()
	{
		Stroke stroke;
		stroke.width = 4.0f;
		return stroke;
	}
}

namespace GISLibrary
{
	void DrawFeatureHighlight(IRenderer& renderer, Scaler& scaler, SGeometry* geometry, float offsetX, float offsetY)
	{
		if (!geometry)
		{
			return;
		}
		renderer.SetTransform({ TransformStep::Translate(offsetX, offsetY) });

		switch (geometry->GetType())
		{
		case SGeometryType::Point:
		{
			auto point = static_cast<SPoint*>(geometry);
			const auto at = OnScreen(scaler, point->x, point->y);
			// A bracket at each corner of a square about the point.
			Path path;
			for (const float sx : { -1.0f, 1.0f })
			{
				for (const float sy : { -1.0f, 1.0f })
				{
					PathFigure bracket;
					bracket.start = { at.x + sx * 20, at.y + sy * 8 };
					bracket.lines = { { at.x + sx * 20, at.y + sy * 20 }, { at.x + sx * 8, at.y + sy * 20 } };
					path.figures.push_back(std::move(bracket));
				}
			}
			renderer.StrokePath(path, kRed, Thick());
			break;
		}
		case SGeometryType::MultiPoint:
		{
			auto points = static_cast<SMultiPoint*>(geometry);
			Path ring;
			ring.arcs.push_back({ 20.0f, 0.0f, 360.0f });
			for (int i = 0; i < points->GetNumPoints(); i++)
			{
				const auto at = OnScreen(scaler, points->GetX(i), points->GetY(i));
				renderer.SetTransform({ TransformStep::Translate(at.x + offsetX, at.y + offsetY) });
				renderer.StrokePath(ring, kRed, Thick());
			}
			break;
		}
		case SGeometryType::CompositeCurve:
		{
			auto composite = static_cast<SCompositeCurve*>(geometry);
			Path path;
			for (int i = 0; i < composite->GetCurveCount(); i++)
			{
				auto curve = composite->GetCurve(i);
				const int count = curve ? curve->getNumPoint() : 0;
				if (count < 2)
				{
					continue;
				}
				PathFigure line;
				line.start = OnScreen(scaler, curve->GetX(0), curve->GetY(0));
				for (int j = 1; j < count; j++)
				{
					line.lines.push_back(OnScreen(scaler, curve->GetX(j), curve->GetY(j)));
				}
				path.figures.push_back(std::move(line));
			}
			renderer.StrokePath(path, kRed, Thick());
			break;
		}
		case SGeometryType::Curve:
		{
			auto curve = static_cast<SCurve*>(geometry);
			const int count = curve->GetNumPoints();
			if (count >= 2)
			{
				PathFigure line;
				line.start = OnScreen(scaler, curve->GetX(0), curve->GetY(0));
				for (int j = 1; j < count; j++)
				{
					line.lines.push_back(OnScreen(scaler, curve->GetX(j), curve->GetY(j)));
				}
				Path path;
				path.figures.push_back(std::move(line));
				renderer.StrokePath(path, kRed, Thick());
			}
			break;
		}
		case SGeometryType::Surface:
		{
			auto surface = static_cast<SSurface*>(geometry);
			Path path;
			for (int part = 0; part < surface->GetNumPart(); part++)
			{
				const int count = surface->GetNumPointPerPart(part);
				if (count < 3)
				{
					continue;
				}
				PathFigure ring;
				const auto first = surface->GetXY(part, 0);
				ring.start = OnScreen(scaler, first.x, first.y);
				for (int j = 1; j < count; j++)
				{
					const auto next = surface->GetXY(part, j);
					ring.lines.push_back(OnScreen(scaler, next.x, next.y));
				}
				ring.closed = true;
				path.figures.push_back(std::move(ring));
			}
			s100::render::Fill fill;
			fill.color = { 1.0f, 50.0f / 255.0f, 50.0f / 255.0f, 0.49f };
			fill.smooth = true;
			renderer.FillPath(path, fill);
			break;
		}
		default:
			break;
		}
		renderer.SetTransform({});
	}

	void DrawVertexHandle(IRenderer& renderer, float x, float y, bool selected)
	{
		PathFigure square;
		square.start = { x - 5, y - 5 };
		square.lines = { { x + 5, y - 5 }, { x + 5, y + 5 }, { x - 5, y + 5 } };
		square.closed = true;
		Path path;
		path.figures.push_back(std::move(square));

		renderer.SetTransform({});
		s100::render::Fill white;
		white.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		renderer.FillPath(path, white);
		Stroke edge;
		edge.width = selected ? 3.0f : 1.0f;
		edge.flatCaps = true;
		renderer.StrokePath(path, selected ? kRed : ColorF{ 0.0f, 0.0f, 0.0f, 1.0f }, edge);
	}

	void DrawInformationBox(IRenderer& renderer, float left, float top, float right, float bottom,
		const std::string& firstLine, const std::string& secondLine)
	{
		PathFigure box;
		box.start = { left, top };
		box.lines = { { right, top }, { right, bottom }, { left, bottom } };
		box.closed = true;
		Path path;
		path.figures.push_back(std::move(box));

		renderer.SetTransform({});
		s100::render::Fill inside;
		inside.color = { 1.0f, 0.0f, 0.0f, 50.0f / 255.0f };
		renderer.FillPath(path, inside);
		Stroke edge;
		edge.width = 1.0f;
		edge.flatCaps = true;
		renderer.StrokePath(path, kRed, edge);

		s100::render::TextFont font;
		font.size = 20.0f;
		const ColorF blue{ 0.0f, 0.0f, 1.0f, 1.0f };
		const float lineHeight = renderer.MeasureText(firstLine, font).height;
		renderer.FillText(firstLine, { left, top }, font, blue);
		renderer.FillText(secondLine, { left, top + lineHeight }, font, blue);
	}
}
