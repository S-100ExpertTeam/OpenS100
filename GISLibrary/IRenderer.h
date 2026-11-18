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
		enum class Kind { Rotate, Translate };

		Kind kind = Kind::Translate;
		float degrees = 0.0f;   // Rotate: clockwise on a surface whose y axis points down
		float x = 0.0f;         // Translate
		float y = 0.0f;

		static TransformStep Rotate(float degrees)
		{
			TransformStep step;
			step.kind = Kind::Rotate;
			step.degrees = degrees;
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

	// How a path is stroked. The look of an S-100 line is fixed: flat ends, mitre joins (limit
	// 10), round ends on the dashes; a backend applies that to every stroke.
	struct Stroke
	{
		float width = 1.0f;
		// Ink and gap lengths in turn, in multiples of the width; empty for a solid line.
		std::vector<float> dashes;
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

		// Replaces the transform; it stays until the next call.
		virtual void SetTransform(const Transform& transform) = 0;
		virtual void StrokePath(const Path& path, const ColorF& color, const Stroke& stroke) = 0;

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
