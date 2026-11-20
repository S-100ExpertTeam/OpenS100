#include "SkiaFonts.h"
#include "SkiaText.h"

#include "include/core/SkData.h"
#include "include/core/SkFontMetrics.h"
#include "include/core/SkFontMgr.h"
#include "include/core/SkFontTypes.h"
#include "include/ports/SkFontMgr_empty.h"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <vector>

namespace
{
	// The next character of a UTF-8 text and how many bytes it takes; a byte that is no
	// character is taken as one of its own.
	int NextCharacter(const std::string& text, size_t at, size_t& length)
	{
		const auto byte = [&text](size_t i) { return static_cast<unsigned char>(text[i]); };
		const unsigned char first = byte(at);
		int count = first < 0x80 ? 1 : (first >> 5) == 0x6 ? 2 : (first >> 4) == 0xE ? 3 : (first >> 3) == 0x1E ? 4 : 0;
		if (count == 0 || at + count > text.size())
		{
			length = 1;
			return 0xFFFD;
		}
		int character = count == 1 ? first : first & (0xFF >> (count + 1));
		for (int i = 1; i < count; i++)
		{
			if ((byte(at + i) & 0xC0) != 0x80)
			{
				length = 1;
				return 0xFFFD;
			}
			character = (character << 6) | (byte(at + i) & 0x3F);
		}
		length = static_cast<size_t>(count);
		return character;
	}

	// A face at a size, set as every text is drawn: grey antialiasing (no LCD stripes, which
	// depend on the screen), glyphs at fractions of a pixel, advances as the font gives them.
	SkFont Sized(const sk_sp<SkTypeface>& face, const s100::render::TextFont& font)
	{
		// The size is cut to whole pixels, as the viewer has always done.
		SkFont sized(face, static_cast<float>(static_cast<int>(font.size)));
		sized.setEdging(SkFont::Edging::kAntiAlias);
		sized.setSubpixel(true);
		sized.setLinearMetrics(true);
		sized.setHinting(SkFontHinting::kNone);
		if (font.italic && !face->isItalic())
		{
			// A face with no italic is slanted by 20 degrees, as DirectWrite slants one.
			sized.setSkewX(-0.36397f);
		}
		return sized;
	}
}

namespace s100::render
{
	SkiaFonts::SkiaFonts() : faces(std::make_unique<Faces>())
	{
	}

	SkiaFonts::~SkiaFonts() = default;

	bool SkiaFonts::Add(const std::filesystem::path& file)
	{
		// Read by the standard library, which takes the path as it is on every platform.
		std::ifstream in(file, std::ios::binary);
		if (!in)
		{
			return false;
		}
		const std::vector<char> bytes((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		if (bytes.empty())
		{
			return false;
		}
		static const sk_sp<SkFontMgr> manager = SkFontMgr_New_Custom_Empty();   // FreeType, no system fonts
		sk_sp<SkTypeface> face = manager ? manager->makeFromData(SkData::MakeWithCopy(bytes.data(), bytes.size())) : nullptr;
		if (!face)
		{
			return false;
		}
		faces->faces.push_back(std::move(face));
		return true;
	}

	size_t SkiaFonts::Size() const
	{
		return faces->faces.size();
	}

	TextLine LayOut(const SkiaFonts::Faces& fonts, const std::string& text, const TextFont& font)
	{
		TextLine line;
		if (fonts.faces.empty())
		{
			return line;
		}

		// The display font gives the line its height, whatever faces the characters come from.
		SkFontMetrics metrics;
		Sized(fonts.faces.front(), font).getMetrics(&metrics);
		line.ascent = -metrics.fAscent;
		line.size.height = -metrics.fAscent + metrics.fDescent + metrics.fLeading;

		const SkTypeface* current = nullptr;
		for (size_t at = 0; at < text.size();)
		{
			size_t length = 1;
			const int character = NextCharacter(text, at, length);

			// The first face that has the character; the display font's missing glyph if none has.
			const sk_sp<SkTypeface>* face = &fonts.faces.front();
			for (const auto& candidate : fonts.faces)
			{
				if (candidate->unicharToGlyph(character) != 0)
				{
					face = &candidate;
					break;
				}
			}
			if (face->get() != current)
			{
				TextRun run;
				run.font = Sized(*face, font);
				line.runs.push_back(std::move(run));
				current = face->get();
			}
			line.runs.back().text.append(text, at, length);
			at += length;
		}

		float width = 0.0f;
		for (TextRun& run : line.runs)
		{
			run.advance = run.font.measureText(run.text.data(), run.text.size(), SkTextEncoding::kUTF8);
			width += run.advance;
		}
		// Spaces at the end move the pen and are no part of the box.
		const size_t end = text.find_last_not_of(' ');
		if (end != std::string::npos && end + 1 < text.size() && !line.runs.empty())
		{
			const TextRun& last = line.runs.back();
			const size_t spaces = std::min(text.size() - end - 1, last.text.size());
			width -= last.font.measureText(last.text.data() + last.text.size() - spaces, spaces, SkTextEncoding::kUTF8);
		}
		line.size.width = end == std::string::npos ? 0.0f : width;
		return line;
	}
}
