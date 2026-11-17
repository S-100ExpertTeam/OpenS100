#pragma once
#include "D2DForward.h"

#include <vector>

class SENC_LineStyleBase;
class SENC_TextInstruction;

// What the Direct2D renderer caches per drawing instruction. The entries live in the render
// cache slot of the model object, so they go when the instruction goes.
namespace GISLibrary::D2DInstructionCache
{
	// The measured size of the text of an instruction; nullptr until NewTextMetrics.
	DWRITE_TEXT_METRICS* TextMetrics(SENC_TextInstruction& instruction);
	DWRITE_TEXT_METRICS& NewTextMetrics(SENC_TextInstruction& instruction);

	// The arcs of an augmented path drawn with a line style, built around the origin. Empty until
	// the caller fills it; the geometries are released with the line style.
	std::vector<ID2D1PathGeometry*>& ArcGeometries(SENC_LineStyleBase& lineStyle);
}
