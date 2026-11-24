#include "stdafx.h"
#include "DisplayListPainter.h"

#include "R_FeatureRecord.h"
#include "SCommonFuction.h"
#include "SCompositeCurve.h"
#include "SCurve.h"
#include "SSurface.h"
#include "SENC_AreaFillReference.h"
#include "SENC_AreaInstruction.h"
#include "SENC_ColorFill.h"
#include "SENC_CompositeLineStyle.h"
#include "SENC_SymbolFill.h"
#include "SENC_LineInstruction.h"
#include "SENC_LineStyleReference.h"
#include "SENC_SpatialReference.h"
#include "SENC_ArcByRadius.h"
#include "SENC_AugmentedPath.h"
#include "SENC_AugmentedRay.h"
#include "SENC_Dash.h"
#include "SENC_LineStyle.h"
#include "SENC_Define.h"
#include "SENC_Path.h"
#include "SENC_PointInstruction.h"
#include "SENC_Symbol.h"
#include "SENC_Sector.h"
#include "SENC_TextInstruction.h"
#include "RenderCacheSlot.h"
#include "SGeometry.h"
#include "SPoint.h"

#include "../GeoMetryLibrary/ENCCommon.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"
#include "../GeoMetryLibrary/Scaler.h"
#include "../LatLonUtility/LatLonUtility.h"

#include <pugixml.hpp>

#include <climits>
#include <cmath>
#include <memory>
#include <utility>

// The arithmetic below is the arithmetic of the Direct2D painter it came from, type for type:
// the pictures are compared byte for byte.
#pragma warning(disable:4244)
#pragma warning(disable:4838)

namespace
{
	// Millimetres of an augmented ray or arc to pixels.
	const float factor = 3.5;

	// Pixels to a millimetre of a symbol.
	const float symbolScale = 5;

	// The path of a curve or a surface as a backend keeps it, parked with the geometry. Drawn
	// by another backend, the geometry gets a path of that one in its place.
	struct KeptPath : Geometry::RenderAttachment
	{
		std::unique_ptr<s100::render::RetainedPath> path;   // nothing when there is nothing to draw
		const void* backend = nullptr;                      // IRenderer::Backend of the one that made it

		// The path with fewer vertices that is drawn at the zoom level named: nothing when the
		// geometry loses too few at that zoom to be worth a path of its own.
		std::unique_ptr<s100::render::RetainedPath> coarse;
		int coarseLevel = noLevel;

		static constexpr int noLevel = INT_MIN;
	};

	// A geometry of fewer vertices than this is always drawn whole.
	constexpr int coarseMinVertices = 32;

	// The width of the grid the vertices are gathered on at a zoom level, in the units of the
	// dataset: a quarter of the size a pixel has at the start of the level, so between an
	// eighth and a quarter of a pixel on the screen.
	double GridOf(int zoomLevel)
	{
		return std::ldexp(0.25, zoomLevel);
	}

	// A run of vertices as one figure, each moved to the nearest node of the grid and left out
	// when it lands on the one before. Which nodes a run passes does not depend on which end
	// it is walked from, so an edge two surfaces share comes out the same for both. A run that
	// falls on a single node is kept as a line of no length, which a round pen draws as a dot.
	template <typename XAt, typename YAt>
	s100::render::PathFigure GatheredFigure(int count, XAt xAt, YAt yAt, double grid, bool closed)
	{
		s100::render::PathFigure figure;
		figure.closed = closed;
		const double perUnit = 1.0 / grid;
		double lastX = 0;
		double lastY = 0;
		for (int i = 0; i < count; i++)
		{
			const double x = std::floor(xAt(i) * perUnit + 0.5) * grid;
			const double y = std::floor(yAt(i) * perUnit + 0.5) * grid;
			if (i == 0)
			{
				figure.start = { static_cast<float>(x), static_cast<float>(-y) };
			}
			else if (x != lastX || y != lastY)
			{
				figure.lines.push_back({ static_cast<float>(x), static_cast<float>(-y) });
			}
			else
			{
				continue;
			}
			lastX = x;
			lastY = y;
		}
		if (count > 1 && figure.lines.empty())
		{
			figure.lines.push_back(figure.start);
		}
		return figure;
	}

	size_t VerticesOf(const s100::render::Path& path)
	{
		size_t vertices = 0;
		for (const auto& figure : path.figures)
		{
			vertices += figure.lines.size() + 1;
		}
		return vertices;
	}

	// The path a geometry of `vertices` vertices is drawn from at a zoom level: its coarse one,
	// made by `build` from the width of the grid when the zoom has changed, or the whole one
	// when the coarse one would keep more than three vertices in four.
	template <typename Build>
	const s100::render::RetainedPath* PathAtZoom(KeptPath& kept, s100::render::IRenderer& renderer, int vertices, int zoomLevel,
		Build build)
	{
		if (!kept.path || vertices < coarseMinVertices || !s100::render::DisplayListPainter::gatherVertices)
		{
			return kept.path.get();
		}
		if (kept.coarseLevel != zoomLevel)
		{
			kept.coarseLevel = zoomLevel;
			kept.coarse.reset();
			const s100::render::Path path = build(GridOf(zoomLevel));
			if (VerticesOf(path) * 4 <= static_cast<size_t>(vertices) * 3)
			{
				kept.coarse = renderer.Retain(path);
			}
		}
		return kept.coarse ? kept.coarse.get() : kept.path.get();
	}

	// The rings of a surface as closed figures; the points are given by a function of the index.
	template <typename PointAt>
	s100::render::Path SurfaceRings(SSurface* surface, PointAt pointAt)
	{
		s100::render::Path path;
		if (surface->m_numPoints < 3)
		{
			return path;
		}
		const int parts = surface->m_numParts <= 1 ? 1 : surface->m_numParts;
		for (int part = 0; part < parts; part++)
		{
			const int start = surface->m_numParts <= 1 ? 0 : surface->m_pParts[part];
			const int count = surface->m_numParts <= 1 ? surface->m_numPoints : surface->GetNumPointPerPart(part);
			if (count < 2)
			{
				continue;
			}
			s100::render::PathFigure ring;
			ring.closed = true;
			ring.start = pointAt(start);
			ring.lines.resize(static_cast<size_t>(count - 1));
			for (int i = 1; i < count; i++)
			{
				ring.lines[i - 1] = pointAt(start + i);
			}
			path.figures.push_back(std::move(ring));
		}
		return path;
	}

	// The measured size of a text, kept in the cache slot of its instruction. Drawn by another
	// backend, the text is measured again: the glyphs are the backend's.
	struct TextSizeEntry : GISLibrary::RenderCacheEntry
	{
		s100::render::SizeF size;
		const void* backend = nullptr;   // IRenderer::Backend of the one that measured
	};
}

namespace s100::render
{
	DisplayListPainter::DisplayListPainter(IRenderer& renderer, Scaler& scaler, ColorLookup color)
		: renderer(renderer), scaler(scaler), color(std::move(color))
	{
	}

	// A symbol at every point the instruction is drawn at (a point feature, the points of a
	// multipoint, the place chosen in a surface), turned as the instruction says.
	void DisplayListPainter::Visit(SENC_PointInstruction& target)
	{
		auto instruction = &target;
		if (!instruction->symbol)
		{
			return;
		}

		points.clear();
		instruction->GetDrawPoints(&scaler, points);

		for (auto pi = points.begin(); pi != points.end(); pi++)
		{
			// A rotation from north (GeographicCRS) starts from where north is at the point,
			// which is not up about a pole.
			float northAngle = 0;
			if (IsPolarProjection())
			{
				double mx = 0;
				double my = 0;
				scaler.DeviceToWorld(static_cast<long>(pi->x), static_cast<long>(pi->y), &mx, &my);
				northAngle = static_cast<float>(NorthAngle(mx, my));
			}

			// rotate Point
			if (scaler.GetRotationDegree())
			{
				double radian = (180 - scaler.GetRotationDegree()) * DEG2RAD;

				float tempX = (float)(scaler.soy - pi->y) * (float)sin(radian) + (float)(scaler.sox - pi->x) * (float)cos(radian);
				float tempY = (float)(scaler.soy - pi->y) * (float)cos(radian) - (float)(scaler.sox - pi->x) * (float)sin(radian);

				pi->x = tempX + (float)scaler.sox;
				pi->y = tempY + (float)scaler.soy;
			}

			float rotation = 0;

			if ((instruction->symbol)->rotation)
			{
				rotation = (float)(instruction->symbol)->rotation;
			}
			if (instruction->symbol->rotationCRS == GEOGRAPHIC_CRS)
			{
				rotation += northAngle;
			}

			renderer.DrawSymbol(instruction->symbol->ReferenceUtf8(), { pi->x, pi->y }, rotation, symbolScale);
		}
	}

	// The surface of the feature filled with a colour, an area fill of the catalogue or a symbol
	// repeated.
	void DisplayListPainter::Visit(SENC_AreaInstruction& instruction)
	{
		auto& fr = instruction.fr;
		auto& areaFill = instruction.areaFill;

		if (!areaFill || !fr || !fr->GetGeometry() || fr->GetGeometry()->GetType() != SGeometryType::Surface)
		{
			return;
		}
		auto surface = (SSurface*)fr->GetGeometry();

		// Zoomed far in, the surface is drawn in device pixels and turned with the view.
		const Transform turned = { TransformStep::RotateAbout(
			(float)scaler.GetRotationDegree(), (float)scaler.sox, (float)scaler.soy) };

		Fill fill;
		switch (areaFill->type)
		{
		case 1:
		{
			auto colorFill = (SENC_ColorFill*)areaFill;
			if (colorFill->color)
			{
				fill.color = color(colorFill->color->token);
				fill.color.a = 1 - colorFill->color->transparency;
				FillSurface(surface, fill, turned);
			}
			break;
		}
		case 2:
		{
			auto areaFillReference = (SENC_AreaFillReference*)areaFill;
			fill.kind = Fill::Kind::AreaFill;
			fill.reference = pugi::as_utf8(areaFillReference->GetFileStem());
			fill.fallback = "QUESMRK1";
			FillSurface(surface, fill, turned);
			break;
		}
		case 4:
		{
			auto symbolFill = (SENC_SymbolFill*)areaFill;
			if (symbolFill->GetFileStem().empty() == false)
			{
				// Read from a file of the catalogue (AreaFills): an area fill by name.
				fill.kind = Fill::Kind::AreaFill;
				fill.reference = pugi::as_utf8(symbolFill->GetFileStem());
			}
			else if (symbolFill->symbol)
			{
				fill.kind = Fill::Kind::SymbolTiles;
				fill.symbol = pugi::as_utf8(symbolFill->symbol->reference);
				fill.v1 = { (float)symbolFill->v1.x, (float)symbolFill->v1.y };
				fill.v2 = { (float)symbolFill->v2.x, (float)symbolFill->v2.y };
				fill.offset = { (float)symbolFill->symbol->offset.x, (float)symbolFill->symbol->offset.y };
			}
			else
			{
				break;
			}
			// Zoomed far in, not turned with the view (as before).
			FillSurface(surface, fill, {});
			break;
		}
		}
	}

	void DisplayListPainter::FillSurface(SSurface* surface, const Fill& fill, const Transform& zoomedIn)
	{
		if (scaler.GetCurrentScale() >= SCommonFuction::NewGeometryScale)
		{
			// The path of the surface in the coordinates of the dataset (x, -y), built once.
			auto kept = dynamic_cast<KeptPath*>(surface->renderAttachment.get());
			if (!kept || kept->backend != renderer.Backend())
			{
				auto attachment = std::make_unique<KeptPath>();
				attachment->backend = renderer.Backend();
				attachment->path = renderer.Retain(SurfaceRings(surface, [surface](int i) {
					return PointF{ static_cast<float>(surface->m_pPoints[i].x), static_cast<float>(-surface->m_pPoints[i].y) };
				}));
				kept = attachment.get();
				surface->renderAttachment = std::move(attachment);
			}

			const RetainedPath* drawn = PathAtZoom(*kept, renderer, surface->m_numPoints, ZoomLevel(), [surface](double grid) {
				Path path;
				const int parts = surface->m_numParts <= 1 ? 1 : surface->m_numParts;
				for (int part = 0; part < parts; part++)
				{
					const int start = surface->m_numParts <= 1 ? 0 : surface->m_pParts[part];
					const int count = surface->m_numParts <= 1 ? surface->m_numPoints : surface->GetNumPointPerPart(part);
					if (count < 2)
					{
						continue;
					}
					const SVertex* points = surface->m_pPoints + start;
					path.figures.push_back(GatheredFigure(count, [points](int i) { return points[i].x; },
						[points](int i) { return points[i].y; }, grid, true));
				}
				return path;
			});

			if (drawn)
			{
				renderer.SetTransform(ViewTransform());
				renderer.FillPath(*drawn, fill);
			}
		}
		else
		{
			const Path path = SurfaceRings(surface, [this, surface](int i) {
				PointF point;
				scaler.WorldToDevice_F(surface->m_pPoints[i].x, surface->m_pPoints[i].y, &point.x, &point.y);
				return point;
			});
			if (!path.figures.empty())
			{
				renderer.SetTransform(zoomedIn);
				renderer.FillPath(path, fill);
				renderer.SetTransform(ViewTransform());
			}
		}
	}

	// The curves of the feature the instruction names (all of them when it names none), each
	// drawn with every line style of the instruction.
	void DisplayListPainter::Visit(SENC_LineInstruction& instruction)
	{
		auto& fr = instruction.fr;
		auto& lineStyles = instruction.lineStyles;
		auto& spatialReference = instruction.spatialReference;

		if (!fr || !fr->GetGeometry())
		{
			return;
		}
		auto geom = fr->GetGeometry();

		std::list<SCurve*> curListCurveLink;

		// Surface
		if (geom->GetType() == SGeometryType::Surface)
		{
			auto surface = (SSurface*)geom;
			surface->GetCurveList(curListCurveLink);
		}
		// Composite Curve
		else if (geom->GetType() == SGeometryType::CompositeCurve)
		{
			auto compositeCurve = (SCompositeCurve*)geom;
			compositeCurve->GetCurveList(curListCurveLink);
		}
		// Curve
		else if (geom->GetType() == SGeometryType::Curve)
		{
			auto curve = (SCurve*)geom;
			for (auto itor = lineStyles.begin(); itor != lineStyles.end(); itor++)
			{
				SENC_LineStyleBase* lineStyleBase = *itor;

				if (curve->isDraw())
				{
					DrawLineStyle(lineStyleBase, curve);
				}
			}
			return;
		}

		if (curListCurveLink.size() == 0)
		{
			return;
		}

		if (spatialReference.size() == 0)
		{
			for (auto i = curListCurveLink.begin(); i != curListCurveLink.end(); i++)
			{
				auto curve = (*i);

				if (curve->isDraw())
				{
					for (auto itor = lineStyles.begin(); itor != lineStyles.end(); itor++)
					{
						SENC_LineStyleBase* lineStyleBase = *itor;
						DrawLineStyle(lineStyleBase, *i);
					}
				}
			}
		}

		for (auto i = spatialReference.begin(); i != spatialReference.end(); i++)
		{
			for (auto j = curListCurveLink.begin(); j != curListCurveLink.end(); j++)
			{
				if ((*i)->reference == (*j)->GetRCID())
				{
					if ((*j)->isDraw())
					{
						for (auto itor = lineStyles.begin(); itor != lineStyles.end(); itor++)
						{
							SENC_LineStyleBase* lineStyleBase = *itor;
							DrawLineStyle(lineStyleBase, *j);
						}
					}
					break;
				}
			}
		}
	}

	void DisplayListPainter::DrawLineStyle(SENC_LineStyleBase* lineStyle, SCurve* curve)
	{
		// A composite style visits its parts with the same curve.
		const auto outer = styleCurve;
		styleCurve = curve;
		lineStyle->Accept(*this);
		styleCurve = outer;
	}

	void DisplayListPainter::Visit(SENC_CompositeLineStyle& lineStyle)
	{
		for (auto i = lineStyle.lineStyles.begin(); i != lineStyle.lineStyles.end(); i++)
		{
			DrawLineStyle(*i, styleCurve);
		}
	}

	// A line style given in the instruction: a pen, dashed or not. One with a name is a style of
	// the catalogue under that name.
	void DisplayListPainter::Visit(SENC_LineStyle& lineStyle)
	{
		auto& dashes = lineStyle.dashes;
		auto& symbols = lineStyle.symbols;
		auto& intervalLength = lineStyle.intervalLength;
		auto& pen_width = lineStyle.pen_width;

		auto name = lineStyle.GetLineStyleName();

		if (name != L"")
		{
			CurveOnDevice(true);
			const ColorF penColor = color(lineStyle.GetPenColorToken());
			renderer.StrokeLineStyle(pugi::as_utf8(name), devicePoints, &penColor);
		}
		else
		{
			auto PEN_COLOR = color(lineStyle.GetPenColorToken());
			auto PEN_WIDTH = (float)((pen_width + 0.01) / 0.32);

			ColorF penColor = PEN_COLOR;
			penColor.a = 1;

			Stroke stroke;
			stroke.width = PEN_WIDTH;

			if (dashes.size() > 0)
			{
				float _dashes[] = { (float)(dashes.front()->length + 0.01) / (float)(0.32 * PEN_WIDTH), (float)(intervalLength - dashes.front()->length + 0.01) / (float)(0.32 * PEN_WIDTH) };
				stroke.dashes.assign(std::begin(_dashes), std::end(_dashes));
				StrokeCurve(penColor, stroke);
			}
			else if (symbols.size() > 0)
			{
				// Symbols along a line with no dashes: not drawn (as before).
			}
			else
			{
				StrokeCurve(penColor, stroke);
			}
		}
	}

	// A line style of the catalogue. (Not turned with the view, as before: see the design notes.)
	void DisplayListPainter::Visit(SENC_LineStyleReference& lineStyle)
	{
		CurveOnDevice(false);
		renderer.StrokeLineStyle(pugi::as_utf8(lineStyle.GetReference()), devicePoints, nullptr);
	}

	void DisplayListPainter::CurveOnDevice(bool turnWithView)
	{
		auto curve = styleCurve;
		auto numPoints = curve->GetNumPoints();
		devicePoints.resize(numPoints > 0 ? numPoints : 0);

		for (int pi = 0; pi < numPoints; pi++)
		{
			scaler.WorldToDevice_F(
				curve->GetX(pi),
				curve->GetY(pi),
				&devicePoints[pi].x,
				&devicePoints[pi].y);
		}

		// Point rotate
		if (turnWithView && scaler.GetRotationDegree())
		{
			double radian = (180 - scaler.GetRotationDegree()) * DEG2RAD;

			for (auto id = 0; id < numPoints; id++)
			{
				auto p = &devicePoints[id];
				float tempX = (float)(scaler.soy - p->y) * (float)sin(radian) + (float)(scaler.sox - p->x) * (float)cos(radian);
				float tempY = (float)(scaler.soy - p->y) * (float)cos(radian) - (float)(scaler.sox - p->x) * (float)sin(radian);

				p->x = (float)tempX + (float)scaler.sox;
				p->y = (float)tempY + (float)scaler.soy;
			}
		}
	}

	int DisplayListPainter::ZoomLevel()
	{
		if (!zoomLevelKnown)
		{
			// The size of a pixel in the units of the dataset, as ViewTransform has it.
			const double xRatio = (scaler.mxMax - scaler.mxMin) / (scaler.sxMax - scaler.sxMin);
			const double yRatio = (scaler.myMax - scaler.myMin) / (scaler.syMax - scaler.syMin);
			const double pixel = xRatio > yRatio ? xRatio : yRatio;
			zoomLevel = pixel > 0 && std::isfinite(pixel) ? static_cast<int>(std::floor(std::log2(pixel))) : 0;
			zoomLevelKnown = true;
		}
		return zoomLevel;
	}

	const Transform& DisplayListPainter::ViewTransform()
	{
		// The view does not move while a painter draws.
		if (viewTransformMade)
		{
			return viewTransform;
		}
		viewTransformMade = true;

		// The map range fits the screen in both directions, so the larger of the two
		// world-per-pixel ratios wins.
		const double xRatio = (scaler.mxMax - scaler.mxMin) / (scaler.sxMax - scaler.sxMin);
		const double yRatio = (scaler.myMax - scaler.myMin) / (scaler.syMax - scaler.syMin);
		const double ratio = 1.0 / (xRatio > yRatio ? xRatio : yRatio);

		const double mox = (scaler.mxMax + scaler.mxMin) / 2;
		const double moy = (scaler.myMax + scaler.myMin) / 2;
		const double sox = (scaler.sxMax + scaler.sxMin) / 2;
		const double soy = (scaler.syMax + scaler.syMin) / 2;

		viewTransform.clear();
		viewTransform.push_back(TransformStep::Translate(static_cast<float>(-mox), static_cast<float>(moy)));
		viewTransform.push_back(TransformStep::Scale(static_cast<float>(ratio)));
		viewTransform.push_back(TransformStep::Translate(static_cast<float>(sox), static_cast<float>(soy)));
		return viewTransform;
	}

	void DisplayListPainter::StrokeCurve(const ColorF& penColor, const Stroke& stroke)
	{
		auto curve = styleCurve;

		if (scaler.GetCurrentScale() >= SCommonFuction::NewGeometryScale)
		{
			// The path of the curve in the coordinates of the dataset (x, -y), built once.
			auto kept = dynamic_cast<KeptPath*>(curve->renderAttachment.get());
			if (!kept || kept->backend != renderer.Backend())
			{
				auto attachment = std::make_unique<KeptPath>();
				attachment->backend = renderer.Backend();
				if (curve->m_numPoints >= 2)
				{
					PathFigure figure;
					figure.start = { static_cast<float>(curve->m_pPoints[0].x), static_cast<float>(-curve->m_pPoints[0].y) };
					figure.lines.resize(static_cast<size_t>(curve->m_numPoints - 1));
					for (int i = 1; i < curve->m_numPoints; i++)
					{
						figure.lines[i - 1].x = static_cast<float>(curve->m_pPoints[i].x);
						figure.lines[i - 1].y = static_cast<float>(-curve->m_pPoints[i].y);
					}
					Path path;
					path.figures.push_back(std::move(figure));
					attachment->path = renderer.Retain(path);
				}
				kept = attachment.get();
				curve->renderAttachment = std::move(attachment);
			}

			const RetainedPath* drawn = PathAtZoom(*kept, renderer, curve->m_numPoints, ZoomLevel(), [curve](double grid) {
				const SVertex* points = curve->m_pPoints;
				Path path;
				path.figures.push_back(GatheredFigure(curve->m_numPoints, [points](int i) { return points[i].x; },
					[points](int i) { return points[i].y; }, grid, false));
				return path;
			});

			if (drawn)
			{
				renderer.SetTransform(ViewTransform());
				renderer.StrokePath(*drawn, penColor, stroke);
			}
		}
		else
		{
			CurveOnDevice(false);
			if (devicePoints.size() >= 2)
			{
				PathFigure figure;
				figure.start = devicePoints[0];
				figure.lines.assign(devicePoints.begin() + 1, devicePoints.end());
				Path path;
				path.figures.push_back(std::move(figure));

				renderer.SetTransform({ TransformStep::RotateAbout(
					(float)scaler.GetRotationDegree(), (float)scaler.sox, (float)scaler.soy) });
				renderer.StrokePath(path, penColor, stroke);
				renderer.SetTransform(ViewTransform());
			}
		}
	}

	// The first element of a text at every point the instruction is drawn at, placed by its
	// alignment and offset about the point.
	void DisplayListPainter::Visit(SENC_TextInstruction& target)
	{
		auto instruction = &target;

		// A text of a feature with no geometry has no place to be drawn at.
		SENC_TextPoint* textPoint = instruction->textPoint;
		if (!textPoint || !instruction->fr || !instruction->fr->GetGeometry())
		{
			return;
		}

		points.clear();
		instruction->GetDrawPoints(&scaler, points);

		auto itorTp = textPoint->elements.begin();

		if (itorTp != textPoint->elements.end())
		{
			SENC_Element* element = *itorTp;

			TextFont font;
			font.size = (int)(element->bodySize * (float)1.358);
			font.italic = !element->font.isUpright();

			const std::string text = pugi::as_utf8(element->text.value);

			// Measured once: the size is kept with the instruction.
			auto measured = dynamic_cast<TextSizeEntry*>(instruction->renderCache.get());
			if (!measured || measured->backend != renderer.Backend())
			{
				auto entry = std::make_unique<TextSizeEntry>();
				entry->backend = renderer.Backend();
				entry->size = renderer.MeasureText(text, font);
				measured = entry.get();
				instruction->renderCache = std::move(entry);
			}

			float width = measured->size.width;
			float height = measured->size.height;

			float offset_x = 0;
			float offset_y = 0;

			// Determine the size of the Offset
			float XOFFS = (float)(((float)textPoint->offset.x / 0.32) * 1.358);
			float YOFFS = (float)(-((float)textPoint->offset.y / 0.32) * 1.358);

			// HJUST
			// CENTRE
			if (textPoint->horizontalAlignment == CENTER)
			{
				offset_x = -width / (float)2.;
			}
			// RIGHT
			else if (textPoint->horizontalAlignment == END)
			{
				offset_x = -width;
			}
			// LEFT
			else
			{
				offset_x = 0;
			}

			// VJUST
			// BOTTOM and TOP (as before: a text aligned at the top is placed as one at the bottom)
			if (textPoint->verticalAlignment == BOTTOM || textPoint->verticalAlignment == TOP)
			{
				offset_y = -height;
			}
			// CENTRE
			else
			{
				offset_y = -height / (float)2.;
			}

			offset_x += XOFFS;
			offset_y += YOFFS;

			// A text with no colour of its own is black.
			ColorF foreground{ 0.0f, 0.0f, 0.0f, 1.0f };
			if (element->hasForeground)
			{
				foreground = color(element->foreground.token);
				foreground.a *= 1 - element->foreground.transparency;
			}

			for (auto itor = points.begin(); itor != points.end(); itor++)
			{
				float x = itor->x + offset_x;
				float y = itor->y + offset_y;

				// rotate Point
				if (scaler.GetRotationDegree())
				{
					double _radian = (180 - scaler.GetRotationDegree()) * DEG2RAD;

					float tempX = (float)(scaler.soy - y) * (float)sin(_radian) + (float)(scaler.sox - x) * (float)cos(_radian);
					float tempY = (float)(scaler.soy - y) * (float)cos(_radian) - (float)(scaler.sox - x) * (float)sin(_radian);

					x = tempX + (float)scaler.sox;
					y = tempY + (float)scaler.soy;
				}

				renderer.SetTransform({});
				renderer.FillText(text, { x, y }, font, foreground);
			}
		}
	}

	// A ray from a point feature: a line of a given length in a given direction from north
	// (a leading line, a sector limit of a light).
	void DisplayListPainter::Visit(SENC_AugmentedRay& instruction)
	{
		auto& fr = instruction.fr;
		auto& lineStyles = instruction.lineStyles;
		auto& direction = instruction.direction;
		auto& length = instruction.length;

		if (nullptr == fr || nullptr == fr->GetGeometry())
		{
			return;
		}

		auto geom = fr->GetGeometry();
		if (geom->GetType() != SGeometryType::Point)
		{
			return;
		}

		auto p = (SPoint*)geom;

		scaler.WorldToDevice(
			p->x,
			p->y,
			&SGeometry::viewPoints[0].x,
			&SGeometry::viewPoints[0].y);

		for (auto i = lineStyles.begin(); i != lineStyles.end(); i++)
		{
			auto lineStyle = *i;
			if (!lineStyle->IsLineStyle())
			{
				continue;   // a reference or a composite style: not drawn (as before)
			}

			SENC_LineStyle* ls = (SENC_LineStyle*)lineStyle;
			int PEN_WIDTH = (int)(ls->pen_width / 0.319);

			PointF pBeginningPt = { (float)SGeometry::viewPoints[0].x, (float)SGeometry::viewPoints[0].y };
			PointF pEndPt;

			// Point rotate
			if (scaler.GetRotationDegree())
			{
				double radian = (180 - scaler.GetRotationDegree()) * DEG2RAD;

				float tempX = (scaler.soy - pBeginningPt.y) * sin(radian) + (scaler.sox - pBeginningPt.x) * cos(radian);
				float tempY = (scaler.soy - pBeginningPt.y) * cos(radian) - (scaler.sox - pBeginningPt.x) * sin(radian);

				pBeginningPt.x = tempX + scaler.sox;
				pBeginningPt.y = tempY + scaler.soy;
			}

			long ix = 0;
			long iy = 0;
			double mx = p->x;
			double my = p->y;

			if (false == ENCCommon::FULL_SECTORS)
			{
				// The direction is from north, which is not up about a pole.
				const double onScreen = direction + NorthAngle(mx, my) - 90;
				ix = pBeginningPt.x + (int)((cos(onScreen * M_PI / 180) * length) * factor);
				iy = pBeginningPt.y + (int)((sin(onScreen * M_PI / 180) * length) * factor);

				// Point rotate
				if (scaler.GetRotationDegree())
				{
					double radian = (180 - scaler.GetRotationDegree()) * DEG2RAD;

					float tempX = (pBeginningPt.y - iy) * sin(radian) + (pBeginningPt.x - ix) * cos(radian);
					float tempY = (pBeginningPt.y - iy) * cos(radian) - (pBeginningPt.x - ix) * sin(radian);

					ix = tempX + pBeginningPt.x;
					iy = tempY + pBeginningPt.y;
				}
			}
			else
			{
				inverseProjection(mx, my);
				auto dest = LatLonUtility::GetDestinationPointRhumbLineMercatorByGeographicLib(my, mx, direction, length);
				projection(dest.lon, dest.lat);
				scaler.WorldToDevice(dest.lon, dest.lat, &ix, &iy);

				// Point rotate
				if (scaler.GetRotationDegree())
				{
					double radian = (180 - scaler.GetRotationDegree()) * DEG2RAD;

					float tempX = (scaler.soy - iy) * sin(radian) + (scaler.sox - ix) * cos(radian);
					float tempY = (scaler.soy - iy) * cos(radian) - (scaler.sox - ix) * sin(radian);

					ix = tempX + scaler.sox;
					iy = tempY + scaler.soy;
				}
			}

			pEndPt.x = (float)ix;
			pEndPt.y = (float)iy;

			PathFigure line;
			line.start = pBeginningPt;
			line.lines.push_back(pEndPt);
			Path path;
			path.figures.push_back(line);

			Stroke stroke;
			stroke.width = PEN_WIDTH;
			if (ls->dashes.size() > 0)
			{
				float dashes[] = { (ls->dashes.front()->length + 0.01) / (0.32 * PEN_WIDTH), (ls->intervalLength - ls->dashes.front()->length + 0.01) / (0.32 * PEN_WIDTH) };
				stroke.dashes.assign(std::begin(dashes), std::end(dashes));
			}

			// The ray is in device pixels: whatever transform the lines before it left must go.
			renderer.SetTransform({});
			renderer.StrokePath(path, color(ls->GetPenColorToken()), stroke);
		}
	}

	// Arcs about a point feature (the sectors of a light), turned to where north is at the point.
	void DisplayListPainter::Visit(SENC_AugmentedPath& instruction)
	{
		auto& fr = instruction.fr;
		auto& path = instruction.path;
		auto& lineStyles = instruction.lineStyles;

		if (!fr || !fr->GetGeometry() || !path)
		{
			return;
		}

		auto geom = fr->GetGeometry();
		if (geom->GetType() != SGeometryType::Point)
		{
			return;
		}

		SPoint* p = (SPoint*)geom;
		scaler.WorldToDevice(p->x, p->y, &SGeometry::viewPoints[0].x, &SGeometry::viewPoints[0].y);

		for (auto itor = lineStyles.begin(); itor != lineStyles.end(); itor++)
		{
			SENC_LineStyleBase* lineStyle = *itor;
			if (lineStyle->type != 1)
			{
				continue;   // a reference or a composite style: not drawn (as before)
			}

			SENC_LineStyle* ls = (SENC_LineStyle*)lineStyle;

			// One path an arc, about the origin with north up.
			std::vector<Path> arcs;
			for (auto _itor = path->arcByRadiuses.begin(); _itor != path->arcByRadiuses.end(); _itor++)
			{
				SENC_ArcByRadius* _arcByRadius = *_itor;
				if (!_arcByRadius->sector)
				{
					continue;
				}

				float displayRadius = _arcByRadius->radius * 3.5;

				// A sector starts from north (up the screen); an arc angle is counted from
				// straight down.
				float startAngle = _arcByRadius->sector->startAngle + 180;
				float endAngle = startAngle + _arcByRadius->sector->angularDistance;

				Path arc;
				arc.arcs.push_back({ displayRadius, startAngle, endAngle });
				arcs.push_back(std::move(arc));
			}

			// The arcs are built from north up: turned to where north is at the point.
			renderer.SetTransform({
				TransformStep::Rotate(static_cast<float>(NorthAngle(p->x, p->y))),
				TransformStep::Translate(SGeometry::viewPoints[0].x, SGeometry::viewPoints[0].y) });

			Stroke stroke;
			stroke.width = ls->pen_width / 0.32;
			if (ls->dashes.size() > 0)
			{
				float dashes[] = {
					(float)(ls->dashes.front()->length + 0.01) / (float)0.32,
					(float)(ls->intervalLength - ls->dashes.front()->length + 0.01) / (float)0.32
				};
				stroke.dashes.assign(std::begin(dashes), std::end(dashes));
			}

			const ColorF penColor = color(ls->GetPenColorToken());
			for (const Path& arc : arcs)
			{
				renderer.StrokePath(arc, penColor, stroke);
			}
		}
	}
}
