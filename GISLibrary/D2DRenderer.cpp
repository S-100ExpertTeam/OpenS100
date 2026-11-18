#include "stdafx.h"
#include "D2DRenderer.h"
#include "D2D1Resources.h"
#include "D2DConvert.h"
#include "PortrayalRenderCache.h"

#include "../S100Engine/S100PCManager.h"

#include <pugixml.hpp>

namespace GISLibrary
{
	D2DRenderer::D2DRenderer(D2D1Resources* d2, PortrayalCatalogue* pc) : d2(d2), pc(pc)
	{
	}

	bool D2DRenderer::Ready() const
	{
		return d2 && d2->Factory() && d2->RenderTarget() && d2->SolidColorBrush() && d2->SolidStrokeStyle();
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
		for (const auto& step : transform)
		{
			const D2D1::Matrix3x2F next = step.kind == s100::render::TransformStep::Kind::Rotate
				? D2D1::Matrix3x2F::Rotation(step.degrees)
				: D2D1::Matrix3x2F::Translation(step.x, step.y);
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
		auto factory = d2->Factory();
		auto rt = d2->RenderTarget();
		auto brush = d2->SolidColorBrush();

		ID2D1PathGeometry* geometry = nullptr;
		if (FAILED(factory->CreatePathGeometry(&geometry)) || !geometry)
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
				brush->SetColor(ToD2D(color));

				if (!stroke.dashes.empty())
				{
					// The look of an S-100 line (see Stroke).
					ID2D1StrokeStyle1* dash = nullptr;
					factory->CreateStrokeStyle(
						D2D1::StrokeStyleProperties1(
							D2D1_CAP_STYLE_FLAT,
							D2D1_CAP_STYLE_FLAT,
							D2D1_CAP_STYLE_ROUND,
							D2D1_LINE_JOIN_MITER,
							10.0f,
							D2D1_DASH_STYLE_CUSTOM,
							0.0f),
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
		}
		geometry->Release();
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
