#include "stdafx.h"
#include "D2DInstructionPainter.h"
#include "D2D1Resources.h"
#include "PortrayalRenderCache.h"
#include "SENC_AreaInstruction.h"
#include "SENC_AugmentedPath.h"
#include "SENC_AugmentedRay.h"
#include "SENC_LineInstruction.h"
#include "SENC_PointInstruction.h"
#include "SENC_TextInstruction.h"

#include "../GeoMetryLibrary/Scaler.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"

namespace GISLibrary
{
	D2DInstructionPainter::D2DInstructionPainter(D2D1Resources* d2, Scaler* scaler, PortrayalCatalogue* pc)
		: d2(d2), scaler(scaler), pc(pc), renderer(d2, pc),
		shared(renderer, *scaler, [pc](const std::wstring& token) { return PortrayalRender::Color(pc, token); })
	{
	}
}

// Every kind of instruction is drawn by the shared painter, on Direct2D through D2DRenderer.
void GISLibrary::D2DInstructionPainter::Visit(SENC_AreaInstruction& instruction)
{
	PaintOnRenderer(instruction);
}

// ---- Line instructions: drawn by the shared painter, on Direct2D through D2DRenderer
void GISLibrary::D2DInstructionPainter::Visit(SENC_LineInstruction& instruction)
{
	PaintOnRenderer(instruction);
}

// ---- Augmented ray and path: drawn by the shared painter, on Direct2D through D2DRenderer
void GISLibrary::D2DInstructionPainter::Visit(SENC_AugmentedRay& instruction)
{
	PaintOnRenderer(instruction);
}

void GISLibrary::D2DInstructionPainter::Visit(SENC_AugmentedPath& instruction)
{
	PaintOnRenderer(instruction);
}

void GISLibrary::D2DInstructionPainter::PaintOnRenderer(SENC_Instruction& instruction)
{
	if (!renderer.Ready())
	{
		return;
	}
	instruction.Accept(shared);
}

// ---- Point instructions: drawn by the shared painter as well
void GISLibrary::D2DInstructionPainter::Visit(SENC_PointInstruction& instruction)
{
	PaintOnRenderer(instruction);
}

// ---- Text instructions: drawn by the shared painter as well
void GISLibrary::D2DInstructionPainter::Visit(SENC_TextInstruction& instruction)
{
	PaintOnRenderer(instruction);
}
