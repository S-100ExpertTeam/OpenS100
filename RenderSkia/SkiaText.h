#pragma once
// Inside the Skia backend: the faces of SkiaFonts and a line of text laid out with them.
#include "SkiaFonts.h"

#include "../GISLibrary/IRenderer.h"

#include "include/core/SkFont.h"
#include "include/core/SkRefCnt.h"
#include "include/core/SkTypeface.h"

#include <string>
#include <vector>

namespace s100::render
{
	struct SkiaFonts::Faces
	{
		std::vector<sk_sp<SkTypeface>> faces;
	};

	// A stretch of a text that one face writes.
	struct TextRun
	{
		SkFont font;
		std::string text;       // UTF-8
		float advance = 0.0f;   // how far the run moves the pen
	};

	// A line of text as runs, by the face each character is found in.
	struct TextLine
	{
		std::vector<TextRun> runs;
		float ascent = 0.0f;    // from the top of the line down to the baseline
		SizeF size;             // the box: the advances without trailing spaces, the height of a line
	};

	// Lays a text out in a font. Nothing (no runs, no size) when there is no face.
	TextLine LayOut(const SkiaFonts::Faces& faces, const std::string& text, const TextFont& font);
}
