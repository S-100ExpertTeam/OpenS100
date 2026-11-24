#pragma once
#include "IRenderer.h"

class PortrayalCatalogue;
struct IDWriteTextFormat;
struct ID2D1Geometry;
struct ID2D1PathGeometry;

namespace GISLibrary
{
	class D2D1Resources;

	// IRenderer on the Direct2D render target of the viewer (the transitional backend,
	// DIRECTIVES 24). It draws with the shared brush and stroke styles of D2D1Resources, as the
	// code it replaces did.
	class D2DRenderer : public s100::render::IRenderer
	{
	public:
		// The catalogue is the one whose symbols are drawn.
		D2DRenderer(D2D1Resources* d2, PortrayalCatalogue* pc);

		// False when the Direct2D resources are not there (nothing can be drawn).
		bool Ready() const;

		const void* Backend() const override;
		void SetTransform(const s100::render::Transform& transform) override;
		void StrokePath(const s100::render::Path& path, const s100::render::ColorF& color,
			const s100::render::Stroke& stroke) override;
		std::unique_ptr<s100::render::RetainedPath> Retain(const s100::render::Path& path) override;
		void StrokePath(const s100::render::RetainedPath& path, const s100::render::ColorF& color,
			const s100::render::Stroke& stroke) override;
		void FillPath(const s100::render::Path& path, const s100::render::Fill& fill) override;
		void FillPath(const s100::render::RetainedPath& path, const s100::render::Fill& fill) override;
		void PushClip(const s100::render::Path& path) override;
		void PopClip() override;
		void StrokeLineStyle(const std::string& reference, const std::vector<s100::render::PointF>& points,
			const s100::render::ColorF* shortStretches) override;
		void DrawSymbol(const std::string& reference, s100::render::PointF at, float rotationDegrees, float scale) override;
		s100::render::SizeF MeasureText(const std::string& text, const s100::render::TextFont& font) override;
		void FillText(const std::string& text, s100::render::PointF topLeft, const s100::render::TextFont& font,
			const s100::render::ColorF& color) override;

	private:
		// Strokes a geometry as a path is stroked (see Stroke).
		void StrokeGeometry(ID2D1Geometry* geometry, const s100::render::ColorF& color, const s100::render::Stroke& stroke);

		// Fills a geometry as a path is filled (see Fill).
		void FillGeometry(ID2D1Geometry* geometry, const s100::render::Fill& fill);
		// A path as a Direct2D geometry; the caller releases it.
		ID2D1PathGeometry* Build(const s100::render::Path& path);

		// The DirectWrite format of a font, with wrapping off.
		IDWriteTextFormat* Format(const s100::render::TextFont& font);

		D2D1Resources* d2;
		PortrayalCatalogue* pc;
		// Whether the transform in force scales (a map drawn in the coordinates of its dataset).
		bool scaled = false;
		// Whether each clip pushed has a layer on the target to pop.
		std::vector<bool> clips;
	};
}
