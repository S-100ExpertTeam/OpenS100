#pragma once
#include "D2DForward.h"

class SENC_TextInstruction;

// What the Direct2D renderer caches per drawing instruction. The entries live in the render
// cache slot of the model object, so they go when the instruction goes.
namespace GISLibrary::D2DInstructionCache
{
	// The measured size of the text of an instruction; nullptr until NewTextMetrics.
	DWRITE_TEXT_METRICS* TextMetrics(SENC_TextInstruction& instruction);
	DWRITE_TEXT_METRICS& NewTextMetrics(SENC_TextInstruction& instruction);
}
