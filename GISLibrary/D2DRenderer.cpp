#include "stdafx.h"
#include "D2DRenderer.h"
#include "D2D1Resources.h"
#include "D2DConvert.h"
#include "PortrayalRenderCache.h"

#include "../S100Engine/AreaFill.h"
#include "../S100Engine/AreaPatternBitmap.h"
#include "../S100Engine/S100PCManager.h"
#include "../S100Engine/S100Render.h"

#include <pugixml.hpp>

namespace
{
	// A path kept as a Direct2D geometry.
	struct RetainedGeometry : s100::render::RetainedPath
	{
		ID2D1Geometry* geometry = nullptr;

		~RetainedGeometry() override
		{
			if (geometry)
			{
				geometry->Release();
			}
		}
	};
}

namespace GISLibrary
{
	D2DRenderer::D2DRenderer(D2D1Resources* d2, PortrayalCatalogue* pc) : d2(d2), pc(pc)
	{
	}

	bool D2DRenderer::Ready() const
	{
		return d2 && d2->Factory() && d2->RenderTarget() && d2->SolidColorBrush() && d2->SolidStrokeStyle();
	}

	const void* D2DRenderer::Backend() const
	{
		static const char direct2D = 0;
		return &direct2D;
	}

	void D2DRenderer::SetTransform(const s100::render::Transform& transform)
	{
		if (!Ready())
		{
			return;
		}
		// Direct2D's own matrices: the same arithmetic the painter code used before it moved.
		D2D1::Matrix3x2F matrix = D2D1::Matrix3x2F::Identity();
		bool first = true;
		scaled = false;
		for (const auto& step : transform)
		{
			scaled = scaled || step.kind == s100::render::TransformStep::Kind::Scale;
			D2D1::Matrix3x2F next;
			switch (step.kind)
			{
			case s100::render::TransformStep::Kind::Rotate:
				next = D2D1::Matrix3x2F::Rotation(step.degrees, D2D1::Point2F(step.x, step.y));
				break;
			case s100::render::TransformStep::Kind::Scale:
				next = D2D1::Matrix3x2F::Scale(step.factor, step.factor);
				break;
			default:
				next = D2D1::Matrix3x2F::Translation(step.x, step.y);
				break;
			}
			matrix = first ? next : matrix * next;
			first = false;
		}
		d2->RenderTarget()->SetTransform(matrix);
	}

	void D2DRenderer::StrokePath(const s100::render::Path& path, const s100::render::ColorF& color,
		const s100::render::Stroke& stroke)
	{
		if (!Ready())
		{
			return;
		}
		ID2D1PathGeometry* geometry = nullptr;
		if (FAILED(d2->Factory()->CreatePathGeometry(&geometry)) || !geometry)
		{
			return;
		}

		ID2D1GeometrySink* sink = nullptr;
		if (SUCCEEDED(geometry->Open(&sink)) && sink)
		{
			sink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
			for (const auto& figure : path.figures)
			{
				sink->BeginFigure(ToD2D(figure.start), D2D1_FIGURE_BEGIN_FILLED);
				for (const auto& to : figure.lines)
				{
					sink->AddLine(ToD2D(to));
				}
				sink->EndFigure(figure.closed ? D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END_OPEN);
			}
			for (const auto& arc : path.arcs)
			{
				// Where the point straight down lands when turned by the two angles.
				const D2D1_POINT_2F down = { 0.0F, arc.radius };
				const D2D1_POINT_2F start = D2D1::Matrix3x2F::Rotation(arc.startDegrees).TransformPoint(down);
				const D2D1_POINT_2F end = D2D1::Matrix3x2F::Rotation(arc.endDegrees).TransformPoint(down);

				sink->BeginFigure(start, D2D1_FIGURE_BEGIN_HOLLOW);
				sink->AddArc(D2D1::ArcSegment(
					end,
					D2D1::SizeF(arc.radius, arc.radius),
					0,
					D2D1_SWEEP_DIRECTION_CLOCKWISE,
					(arc.endDegrees - arc.startDegrees) > 180 ? D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE_SMALL));
				sink->EndFigure(D2D1_FIGURE_END_OPEN);
			}
			const HRESULT closed = sink->Close();
			sink->Release();

			if (SUCCEEDED(closed))
			{
				StrokeGeometry(geometry, color, stroke);
			}
		}
		geometry->Release();
	}

	void D2DRenderer::StrokeGeometry(ID2D1Geometry* geometry, const s100::render::ColorF& color, const s100::render::Stroke& stroke)
	{
		auto rt = d2->RenderTarget();
		auto brush = d2->SolidColorBrush();
		brush->SetColor(ToD2D(color));

		if (!stroke.dashes.empty())
		{
			// The look of a dashed line (see Stroke); the width is not scaled by the transform.
			ID2D1StrokeStyle1* dash = nullptr;
			d2->Factory()->CreateStrokeStyle(
				D2D1::StrokeStyleProperties1(
					D2D1_CAP_STYLE_FLAT,
					D2D1_CAP_STYLE_FLAT,
					D2D1_CAP_STYLE_ROUND,
					D2D1_LINE_JOIN_MITER,
					10.0f,
					D2D1_DASH_STYLE_CUSTOM,
					0.0f,
					D2D1_STROKE_TRANSFORM_TYPE_FIXED),
				stroke.dashes.data(),
				static_cast<UINT32>(stroke.dashes.size()),
				&dash);
			rt->DrawGeometry(geometry, brush, stroke.width, dash);
			if (dash)
			{
				dash->Release();
			}
		}
		else
		{
			rt->DrawGeometry(geometry, brush, stroke.width, d2->SolidStrokeStyle());
		}
	}

	ID2D1PathGeometry* D2DRenderer::Build(const s100::render::Path& path)
	{
		bool drawable = !path.arcs.empty();
		for (const auto& figure : path.figures)
		{
			drawable = drawable || !figure.lines.empty();
		}
		if (!drawable)
		{
			return nullptr;
		}

		ID2D1PathGeometry* geometry = nullptr;
		if (FAILED(d2->Factory()->CreatePathGeometry(&geometry)) || !geometry)
		{
			return nullptr;
		}
		ID2D1GeometrySink* sink = nullptr;
		if (FAILED(geometry->Open(&sink)) || !sink)
		{
			geometry->Release();
			return nullptr;
		}
		sink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);
		for (const auto& figure : path.figures)
		{
			if (figure.lines.empty())
			{
				continue;
			}
			sink->BeginFigure(ToD2D(figure.start), figure.closed ? D2D1_FIGURE_BEGIN_FILLED : D2D1_FIGURE_BEGIN_HOLLOW);
			sink->AddLines(reinterpret_cast<const D2D1_POINT_2F*>(figure.lines.data()), static_cast<UINT32>(figure.lines.size()));
			sink->EndFigure(figure.closed ? D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END_OPEN);
		}
		sink->Close();
		sink->Release();
		return geometry;
	}

	std::unique_ptr<s100::render::RetainedPath> D2DRenderer::Retain(const s100::render::Path& path)
	{
		if (!Ready())
		{
			return nullptr;
		}
		ID2D1PathGeometry* geometry = Build(path);
		if (!geometry)
		{
			return nullptr;
		}
		auto retained = std::make_unique<RetainedGeometry>();
		retained->geometry = geometry;
		return retained;
	}

	void D2DRenderer::FillPath(const s100::render::Path& path, const s100::render::Fill& fill)
	{
		if (!Ready())
		{
			return;
		}
		if (ID2D1PathGeometry* geometry = Build(path))
		{
			FillGeometry(geometry, fill);
			geometry->Release();
		}
	}

	void D2DRenderer::FillPath(const s100::render::RetainedPath& path, const s100::render::Fill& fill)
	{
		auto retained = dynamic_cast<const RetainedGeometry*>(&path);
		if (!Ready() || !retained || !retained->geometry)
		{
			return;   // a path of another backend is not drawn
		}
		FillGeometry(retained->geometry, fill);
	}

	void D2DRenderer::FillGeometry(ID2D1Geometry* geometry, const s100::render::Fill& fill)
	{
		auto rt = d2->RenderTarget();

		// A pattern stays in device pixels: its brush undoes the scaling of the map.
		D2D1::Matrix3x2F brushTransform = D2D1::Matrix3x2F::Identity();
		if (scaled)
		{
			rt->GetTransform(&brushTransform);
			brushTransform.Invert();
		}

		rt->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		switch (fill.kind)
		{
		case s100::render::Fill::Kind::Color:
		{
			auto brush = d2->SolidColorBrush();
			brush->SetColor(ToD2D(fill.color));
			rt->FillGeometry(geometry, brush);
			break;
		}
		case s100::render::Fill::Kind::AreaFill:
		{
			auto manager = PortrayalRender::Caches(pc);
			if (!manager)
			{
				break;
			}
			auto& patterns = manager->areaFillInfo.patternMap;
			auto found = patterns.find(pugi::as_wide(fill.reference));
			if (found == patterns.end() && !fill.fallback.empty())
			{
				found = patterns.find(pugi::as_wide(fill.fallback));
			}
			if (found != patterns.end() && found->second->pBitmapBrush)
			{
				found->second->pBitmapBrush->SetTransform(brushTransform);
				rt->FillGeometry(geometry, found->second->pBitmapBrush);
			}
			break;
		}
		case s100::render::Fill::Kind::SymbolTiles:
		{
			auto manager = PortrayalRender::Caches(pc);
			if (!manager)
			{
				break;
			}
			// The tile is drawn from the symbol each time (as before).
			AreaFill tiles;
			tiles.v1_x = fill.v1.x;
			tiles.v1_y = fill.v1.y;
			tiles.v2_x = fill.v2.x;
			tiles.v2_y = fill.v2.y;
			tiles._symbolReference = fill.symbol;
			tiles.offset_x = fill.offset.x;
			tiles.offset_y = fill.offset.y;

			AreaPatternBitmap* image = manager->CreateBitmapImage(&tiles, d2->Factory(), d2->pImagingFactory, d2->SolidStrokeStyle());
			if (image)
			{
				image->pBitmapBrush = manager->CreateBitmapBrush(image, rt);
				if (image->pBitmapBrush)
				{
					image->pBitmapBrush->SetTransform(brushTransform);
					rt->FillGeometry(geometry, image->pBitmapBrush);
				}
				delete image;
			}
			break;
		}
		}
		rt->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	}

	void D2DRenderer::StrokePath(const s100::render::RetainedPath& path, const s100::render::ColorF& color,
		const s100::render::Stroke& stroke)
	{
		auto retained = dynamic_cast<const RetainedGeometry*>(&path);
		if (!Ready() || !retained || !retained->geometry)
		{
			return;   // a path of another backend is not drawn
		}
		StrokeGeometry(retained->geometry, color, stroke);
	}

	void D2DRenderer::PushClip(const s100::render::Path& path)
	{
		// A layer masked by the path; Direct2D carries the mask through the transform in force.
		bool pushed = false;
		if (Ready())
		{
			if (ID2D1PathGeometry* geometry = Build(path))
			{
				auto rt = d2->RenderTarget();
				ID2D1Layer* layer = nullptr;
				if (SUCCEEDED(rt->CreateLayer(&layer)) && layer)
				{
					rt->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), geometry, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE), layer);
					layer->Release();
					pushed = true;
				}
				geometry->Release();
			}
		}
		clips.push_back(pushed);
	}

	void D2DRenderer::PopClip()
	{
		if (clips.empty())
		{
			return;
		}
		if (clips.back())
		{
			d2->RenderTarget()->PopLayer();
		}
		clips.pop_back();
	}

	void D2DRenderer::StrokeLineStyle(const std::string& reference, const std::vector<s100::render::PointF>& points,
		const s100::render::ColorF* shortStretches)
	{
		if (!Ready() || points.empty())
		{
			return;
		}
		auto manager = PortrayalRender::Caches(pc);
		if (!manager)
		{
			return;
		}

		// A style the catalogue does not have is drawn as its question mark.
		IWICBitmap* image = manager->GetLineImage(reference + ".xml");
		if (!image)
		{
			image = manager->GetLineImage(std::string("QUESMRK1.xml"));
			if (!image)
			{
				return;
			}
		}

		auto rt = d2->RenderTarget();
		ID2D1Bitmap* bitmap = nullptr;
		if (FAILED(rt->CreateBitmapFromWicBitmap(image, &bitmap)) || !bitmap)
		{
			return;
		}

		// PointF has the layout of D2D1_POINT_2F (DrawTypes.h); the points are only read.
		auto d2Points = const_cast<D2D1_POINT_2F*>(reinterpret_cast<const D2D1_POINT_2F*>(points.data()));
		const D2D1_RECT_F view = D2D1::RectF(0.0f, 0.0f, rt->GetSize().width, rt->GetSize().height);
		if (shortStretches)
		{
			S100Render::DrawBitmapOnPolyline(rt, d2Points, static_cast<int>(points.size()), bitmap, view,
				ToD2D(*shortStretches), d2->D2D1StrokeStyleGroup.at(1));
		}
		else
		{
			S100Render::DrawBitmapOnPolyline(rt, d2Points, static_cast<int>(points.size()), bitmap, view);
		}
		bitmap->Release();
	}

	void D2DRenderer::DrawSymbol(const std::string& reference, s100::render::PointF at, float rotationDegrees, float scale)
	{
		if (!Ready())
		{
			return;
		}
		auto symbols = PortrayalRender::Caches(pc);
		if (!symbols)
		{
			return;
		}
		auto rt = d2->RenderTarget();

		// The symbol sets a transform of its own.
		D2D1::Matrix3x2F oldTransform;
		rt->GetTransform(&oldTransform);

		symbols->Draw(
			reference,
			rt,
			d2->SolidColorBrush(),
			d2->SolidStrokeStyle(),
			ToD2D(at),
			rotationDegrees,
			scale);

		rt->SetTransform(oldTransform);
	}

	IDWriteTextFormat* D2DRenderer::Format(const s100::render::TextFont& font)
	{
		const int size = static_cast<int>(font.size);
		IDWriteTextFormat* format = font.italic ? d2->getSlantWriteTextFormat(size) : d2->getWriteTextFormat(size);
		if (format)
		{
			format->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
		}
		return format;
	}

	s100::render::SizeF D2DRenderer::MeasureText(const std::string& text, const s100::render::TextFont& font)
	{
		if (!Ready() || !d2->pDWriteFactory)
		{
			return {};
		}
		IDWriteTextFormat* format = Format(font);
		if (!format)
		{
			return {};
		}

		const std::wstring wide = pugi::as_wide(text);
		const D2D1_SIZE_F surface = d2->RenderTarget()->GetSize();

		IDWriteTextLayout* layout = nullptr;
		d2->pDWriteFactory->CreateTextLayout(
			wide.c_str(),
			static_cast<UINT32>(wide.size()),
			format,
			surface.width,
			surface.height,
			&layout);
		if (!layout)
		{
			return {};
		}

		DWRITE_TEXT_METRICS metrics{};
		layout->GetMetrics(&metrics);
		layout->Release();
		return { metrics.width, metrics.height };
	}

	void D2DRenderer::FillText(const std::string& text, s100::render::PointF topLeft, const s100::render::TextFont& font,
		const s100::render::ColorF& color)
	{
		if (!Ready())
		{
			return;
		}
		IDWriteTextFormat* format = Format(font);
		if (!format)
		{
			return;
		}
		auto rt = d2->RenderTarget();
		auto brush = d2->SolidColorBrush();
		brush->SetColor(ToD2D(color));

		// The text is not wrapped and starts at the corner, so the far edges of the box are of no
		// account: the surface is box enough.
		const std::wstring wide = pugi::as_wide(text);
		const D2D1_SIZE_F surface = rt->GetSize();
		rt->DrawText(
			wide.c_str(),
			static_cast<UINT32>(wide.size()),
			format,
			D2D1::RectF(topLeft.x, topLeft.y, topLeft.x + surface.width, topLeft.y + surface.height),
			brush);
	}
}
