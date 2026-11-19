#pragma once
#include "D2DRenderer.h"
#include "DisplayListPainter.h"
#include "DrawTypes.h"
#include "SENC_InstructionVisitor.h"

#include <list>

class PortrayalCatalogue;
class Scaler;
class SCurve;
class SENC_Instruction;
class SENC_LineStyleBase;

namespace GISLibrary
{
	class D2D1Resources;

	// Draws drawing instructions with Direct2D. Everything Direct2D does with an instruction is
	// here; the instruction model only describes what is to be drawn.
	//
	// The transform, the brush opacity and the depth suppression runs are set by the caller
	// (LayerManager::AddSymbolDrawing) between the groups of instructions, as before.
	class D2DInstructionPainter : public SENC_InstructionVisitor
	{
	public:
		D2DInstructionPainter(D2D1Resources* d2, Scaler* scaler, PortrayalCatalogue* pc);

		void Visit(SENC_PointInstruction& instruction) override;
		void Visit(SENC_LineInstruction& instruction) override;
		void Visit(SENC_AreaInstruction& instruction) override;
		void Visit(SENC_TextInstruction& instruction) override;
		void Visit(SENC_AugmentedRay& instruction) override;
		void Visit(SENC_AugmentedPath& instruction) override;

	private:
		// The kinds DisplayListPainter has taken over: drawn by it on a D2DRenderer.
		void PaintOnRenderer(SENC_Instruction& instruction);

		D2D1Resources* d2;
		Scaler* scaler;
		PortrayalCatalogue* pc;

		// The backend-neutral painter and the Direct2D surface it draws on.
		D2DRenderer renderer;
		s100::render::DisplayListPainter shared;
	};
}
