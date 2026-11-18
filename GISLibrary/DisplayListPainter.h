#pragma once
#include "IRenderer.h"
#include "SENC_InstructionVisitor.h"

#include <functional>
#include <list>
#include <string>

class Scaler;

namespace s100::render
{
	// A colour token of the portrayal catalogue in the palette in use.
	using ColorLookup = std::function<ColorF(const std::wstring& token)>;

	// Draws drawing instructions on an IRenderer: the S-100 side of drawing, with no backend in
	// it. It works out where things go on the screen (projection, rotation of the view, north
	// at the point) and how they are stroked, and hands the backend paths.
	//
	// It visits the kinds of instruction it has taken over from D2DInstructionPainter; the
	// others it leaves alone (the visitor's default), so the Direct2D painter forwards exactly
	// those kinds here. Taken over so far: point (symbol), augmented ray, augmented path.
	class DisplayListPainter : public SENC_InstructionVisitor
	{
	public:
		DisplayListPainter(IRenderer& renderer, Scaler& scaler, ColorLookup color);

		void Visit(SENC_PointInstruction& instruction) override;
		void Visit(SENC_AugmentedRay& instruction) override;
		void Visit(SENC_AugmentedPath& instruction) override;

	private:
		IRenderer& renderer;
		Scaler& scaler;
		ColorLookup color;
		// Where a point instruction is drawn; kept to reuse its storage.
		std::list<PointF> points;
	};
}
