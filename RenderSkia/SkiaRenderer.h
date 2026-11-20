#pragma once
// The Skia backend of IRenderer (Documents/TechDocs/2026-11-19-r6-skia-backend-design.md).
//
// It draws on a canvas it is handed and knows nothing else of the surface: whether the pixels
// are in memory or on a GPU, and where they go afterwards, is the application's business.
//
// What it draws so far: paths stroked (solid, dashed, arcs) and filled with a colour. Symbols,
// line styles, pattern fills and texts are counted and left out (NotDrawn); they come with the
// neutral symbol model and the fonts.
#include "../GISLibrary/IRenderer.h"

#include "include/core/SkMatrix.h"

class SkCanvas;
class SkPath;

namespace s100::render
{
	class SkiaRenderer final : public IRenderer
	{
	public:
		// The canvas outlives the renderer. Its matrix is taken as device pixels: the renderer
		// draws under that matrix and leaves it as it found it.
		explicit SkiaRenderer(SkCanvas& canvas);

		const void* Backend() const override;

		void SetTransform(const Transform& transform) override;
		void StrokePath(const Path& path, const ColorF& color, const Stroke& stroke) override;
		std::unique_ptr<RetainedPath> Retain(const Path& path) override;
		void StrokePath(const RetainedPath& path, const ColorF& color, const Stroke& stroke) override;
		void FillPath(const Path& path, const Fill& fill) override;
		void FillPath(const RetainedPath& path, const Fill& fill) override;
		void StrokeLineStyle(const std::string& reference, const std::vector<PointF>& points,
			const ColorF* shortStretches) override;
		void DrawSymbol(const std::string& reference, PointF at, float rotationDegrees, float scale) override;
		SizeF MeasureText(const std::string& text, const TextFont& font) override;
		void FillText(const std::string& text, PointF topLeft, const TextFont& font, const ColorF& color) override;

		// What the painter asked for and this backend does not draw yet.
		struct Omitted
		{
			int symbols = 0;
			int lineStyles = 0;
			int patternFills = 0;
			int texts = 0;
		};

		const Omitted& NotDrawn() const
		{
			return omitted;
		}

	private:
		void Stroke_(const SkPath& path, const ColorF& color, const Stroke& stroke);
		void Fill_(const SkPath& path, const Fill& fill);

		SkCanvas& canvas;
		SkMatrix matrix;   // the transform of the painter, onto device pixels
		Omitted omitted;
	};
}
