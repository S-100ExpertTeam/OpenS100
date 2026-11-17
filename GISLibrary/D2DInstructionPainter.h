#pragma once
#include "DrawTypes.h"
#include "SENC_InstructionVisitor.h"

#include <list>

class PortrayalCatalogue;
class Scaler;
class SCurve;
class SENC_LineStyleBase;

namespace GISLibrary
{
	class D2D1Resources;

	// Draws drawing instructions with Direct2D. Everything Direct2D does with an instruction is
	// here; the instruction model only describes what is to be drawn.
	//
	// The transform, the brush opacity and the depth suppression runs are set by the caller
	// (LayerManager::AddSymbolDrawing) between the groups of instructions, as before.
	class D2DInstructionPainter : public SENC_InstructionVisitor, private SENC_LineStyleVisitor
	{
	public:
		D2DInstructionPainter(D2D1Resources* d2, Scaler* scaler, PortrayalCatalogue* pc);

		void Visit(SENC_PointInstruction& target) override;
		void Visit(SENC_LineInstruction& instruction) override;
		void Visit(SENC_AreaInstruction& instruction) override;
		void Visit(SENC_TextInstruction& target) override;
		void Visit(SENC_AugmentedRay& instruction) override;
		void Visit(SENC_AugmentedPath& instruction) override;

	private:
		void Visit(SENC_LineStyle& lineStyle) override;
		void Visit(SENC_LineStyleReference& lineStyle) override;
		void Visit(SENC_CompositeLineStyle& lineStyle) override;

		// Draws one curve with a line style, whatever kind the style is.
		void DrawLineStyle(SENC_LineStyleBase* lineStyle, SCurve* curve);

		D2D1Resources* d2;
		Scaler* scaler;
		PortrayalCatalogue* pc;

		// The curve the line style being visited is drawn along.
		SCurve* styleCurve = nullptr;
		// Where a point or text instruction is drawn; kept to reuse its storage.
		std::list<PointF> points;
	};
}
