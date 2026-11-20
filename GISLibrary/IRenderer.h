#pragma once
// The drawing surface the S-100 painter draws on (DIRECTIVES 24).
//
// DisplayListPainter decides what goes where: it projects, lays out dashes and symbols, resolves
// colour tokens. What is left for a backend is the primitive: stroke this path with this colour,
// put this symbol here, measure and draw this line of text.
// A backend implements IRenderer and knows nothing of S-100; the painter knows nothing of the
// backend. Direct2D is the first backend (D2DRenderer); tests record the calls.
//
// Angles and transforms are handed over as what they mean (turn by so many degrees, an arc
// from this angle to that), not as matrices and end points worked out here: every backend has
// its own trigonometry, and the last bit of a sine decides whether a picture is the same byte
// for byte as before. Direct2D's rotation matrix, for one, cannot be reproduced outside it.
//
// The interface grows with the painter: each kind of instruction that moves from
// D2DInstructionPainter to DisplayListPainter brings the primitives it needs.
#include "DrawTypes.h"

#include <memory>
#include <string>
#include <vector>

namespace s100::render
{
	using GISLibrary::ColorF;
	using GISLibrary::PointF;

	// One step of a transform. Steps apply in order: { Rotate(a), Translate(x, y) } turns about
	// the origin first and moves after.
	struct TransformStep
	{
		enum class Kind { Rotate, Translate, Scale };

		Kind kind = Kind::Translate;
		float degrees = 0.0f;   // Rotate: clockwise on a surface whose y axis points down
		float x = 0.0f;         // Translate: how far. Rotate: the point turned about
		float y = 0.0f;
		float factor = 1.0f;    // Scale: about the origin, the same in both directions

		static TransformStep Rotate(float degrees)
		{
			TransformStep step;
			step.kind = Kind::Rotate;
			step.degrees = degrees;
			return step;
		}

		static TransformStep RotateAbout(float degrees, float x, float y)
		{
			TransformStep step = Rotate(degrees);
			step.x = x;
			step.y = y;
			return step;
		}

		static TransformStep Scale(float factor)
		{
			TransformStep step;
			step.kind = Kind::Scale;
			step.factor = factor;
			return step;
		}

		static TransformStep Translate(float x, float y)
		{
			TransformStep step;
			step.x = x;
			step.y = y;
			return step;
		}
	};

	// No step is the identity: device pixels.
	using Transform = std::vector<TransformStep>;

	// Straight lines from a start point.
	struct PathFigure
	{
		PointF start;
		std::vector<PointF> lines;
		bool closed = false;
	};

	// An arc of a circle about the origin. An angle is where the point (0, radius), straight
	// down the screen, lands when turned clockwise by it; the arc runs clockwise from
	// startDegrees to endDegrees.
	struct ArcFigure
	{
		float radius = 0.0f;
		float startDegrees = 0.0f;
		float endDegrees = 0.0f;
	};

	// What is stroked in one call, in the coordinates of the current transform.
	struct Path
	{
		std::vector<PathFigure> figures;
		std::vector<ArcFigure> arcs;
	};

	// A path a backend keeps in its own form (IRenderer::Retain), so that a curve of a dataset
	// is built once and stroked every frame. It belongs to the backend that made it.
	class RetainedPath
	{
	public:
		virtual ~RetainedPath() = default;
	};

	// How a path is stroked. The look is fixed, as the viewer has drawn it: a solid line has
	// round ends and round joins; a dashed line has flat ends, mitre joins (limit 10) and round
	// ends on the dashes. A backend applies that to every stroke.
	struct Stroke
	{
		// In device pixels, whatever the transform: a line is as wide on a map scaled a
		// thousand times as on one drawn in pixels.
		float width = 1.0f;
		// Ink and gap lengths in turn, in multiples of the width; empty for a solid line.
		std::vector<float> dashes;
	};

	// What a path is filled with.
	//
	// A pattern is a tile repeated over the surface. It is laid in device pixels from the corner
	// of the surface, whatever the transform the path is drawn under, so the patterns of two
	// areas side by side run on. As with a symbol, the backend owns what a tile looks like.
	struct Fill
	{
		enum class Kind
		{
			Color,       // one colour
			AreaFill,    // an area fill of the portrayal catalogue, by name
			SymbolTiles  // a symbol of the catalogue repeated on a lattice
		};

		Kind kind = Kind::Color;
		ColorF color;

		// Antialiased edges: for a shape on its own (a part of a symbol). Areas of a chart are
		// filled without, so that two side by side show no seam.
		bool smooth = false;

		// AreaFill: the name, and the area fill drawn when the catalogue has none of that name
		// (empty: nothing is drawn then).
		std::string reference;
		std::string fallback;

		// SymbolTiles: the symbol, the two vectors of the lattice and where the symbol sits in
		// a tile, in millimetres.
		std::string symbol;
		PointF v1;
		PointF v2;
		PointF offset;
	};

	// The size of something on the surface, in device pixels.
	struct SizeF
	{
		float width = 0.0f;
		float height = 0.0f;
	};

	// The font of a text. The typeface is the backend's (the display font of the application);
	// the painter says how large and whether slanted.
	struct TextFont
	{
		float size = 0.0f;      // the height of the font, in device pixels
		bool italic = false;
	};

	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		// What tells one backend from another; the same for every renderer of a backend. What a
		// backend made for the painter (a kept path, the measured size of a text) stays with
		// the data, and is used again only by a renderer of the backend that made it.
		virtual const void* Backend() const = 0;

		// Replaces the transform; it stays until the next call.
		virtual void SetTransform(const Transform& transform) = 0;
		virtual void StrokePath(const Path& path, const ColorF& color, const Stroke& stroke) = 0;

		// A path to keep: nothing if it has nothing to draw (a single point).
		virtual std::unique_ptr<RetainedPath> Retain(const Path& path) = 0;
		virtual void StrokePath(const RetainedPath& path, const ColorF& color, const Stroke& stroke) = 0;

		// The inside of a path by the even-odd rule (a ring inside a ring is a hole), without
		// antialiasing unless the fill asks for it (Fill::smooth). A figure left open is filled
		// as if it were closed.
		virtual void FillPath(const Path& path, const Fill& fill) = 0;
		virtual void FillPath(const RetainedPath& path, const Fill& fill) = 0;

		// A line style of the portrayal catalogue (a strip repeated along the line) along a
		// polyline given in device pixels, whatever the transform. As with a symbol, the
		// backend owns what the style looks like. With a colour, a stretch too short for the
		// strip is a thin dashed line in it. The transform is the same after the call as before.
		virtual void StrokeLineStyle(const std::string& reference, const std::vector<PointF>& points,
			const ColorF* shortStretches) = 0;

		// A symbol of the portrayal catalogue with its pivot at a point of the surface (device
		// pixels, whatever the transform), turned clockwise by so many degrees. The scale is
		// pixels to a millimetre of the symbol. The backend owns what a symbol looks like (it
		// built it from the SVG of the catalogue) and the palette it is coloured in. The
		// transform is the same after the call as before.
		virtual void DrawSymbol(const std::string& reference, PointF at, float rotationDegrees, float scale) = 0;

		// The box a line of text (UTF-8, never wrapped) takes in a font. The painter places a
		// text by it (centred, right aligned), so a backend answers for the glyphs it will draw.
		virtual SizeF MeasureText(const std::string& text, const TextFont& font) = 0;

		// A line of text with the top left corner of its box at a point, in the coordinates of
		// the current transform. (Not DrawText: that name is a macro of the Windows headers.)
		virtual void FillText(const std::string& text, PointF topLeft, const TextFont& font, const ColorF& color) = 0;
	};
}
