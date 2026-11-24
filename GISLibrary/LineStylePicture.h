#pragma once
// A line style of a portrayal catalogue as dashes and symbols, with no backend in it, and the
// drawing of it along a line on an IRenderer
// (Documents/TechDocs/2026-11-19-r6-skia-backend-design.md, 3.2).
//
// A line style of the catalogue (LineStyles/*.xml) is a strip repeated along the line: within
// one interval, dashes (from where, how long) and symbols (where, turned how). A composite line
// style is several lines side by side, each at an offset from the line.
//
// The strip is laid along every stretch of a polyline from its first point, turned to the
// stretch; it starts again at every vertex. What is left of the last interval of a stretch is
// drawn as far as the stretch goes: a dash is cut there, a symbol is drawn whole if its place
// is still on the stretch.
//
// Lengths are in device pixels, as the viewer has always scaled them: 6.25 pixels to a
// millimetre along the line (dashes, places of symbols), symbols at the symbol scale of the
// display, a dash one pixel wide whatever the pen says.
#include "DisplayListPainter.h"
#include "IRenderer.h"

#include <filesystem>
#include <map>
#include <string>
#include <vector>

class PortrayalCatalogue;

namespace s100::render
{
	class SymbolLibrary;

	struct LineStylePicture
	{
		std::string name;         // the file name without its extension

		// The interval the strip repeats at, in whole pixels; 0 for a composite line style.
		float length = 0.0f;

		// The pen of the dashes.
		std::string colorToken;
		float opacity = 1.0f;

		struct Dash
		{
			float start = 0.0f;   // pixels from the start of the interval
			float length = 0.0f;
		};
		std::vector<Dash> dashes;

		struct Symbol
		{
			std::string reference;
			float position = 0.0f;   // pixels from the start of the interval
			float rotation = 0.0f;   // degrees clockwise from the direction of the line
		};
		std::vector<Symbol> symbols;

		// A composite line style: lines along the whole stretch, side by side.
		struct Line
		{
			float offset = 0.0f;     // pixels to the right of the line, looking along it
			float width = 1.0f;      // pixels
			std::string colorToken;
			float opacity = 1.0f;
		};
		std::vector<Line> lines;
	};

	// Reads a line style file. False if it has no line style.
	bool ReadLineStyle(const std::filesystem::path& path, LineStylePicture& lineStyle);

	// The line styles of a catalogue by name, read when the library is made: they are few.
	class LineStyleLibrary
	{
	public:
		explicit LineStyleLibrary(PortrayalCatalogue& catalogue);

		const LineStylePicture* Find(const std::string& name) const;

		size_t Size() const
		{
			return lineStyles.size();
		}

	private:
		std::map<std::string, LineStylePicture> lineStyles;
	};

	// A stretch shorter than this, in pixels, is a thin dashed line when a colour is given for it.
	inline constexpr float kShortStretch = 20.0f;

	// Draws a line style along a polyline given in device pixels. With a colour, a stretch too
	// short for the strip is a thin dashed line in it. Sets the transform of the renderer and
	// leaves it set.
	void DrawLineStyle(IRenderer& renderer, const LineStylePicture& lineStyle, const std::vector<PointF>& points,
		const ColorF* shortStretches, SymbolLibrary& symbols, const ColorLookup& color);

	// Draws the line style of a name as a display does: the question mark of the catalogue
	// (QUESMRK1) for a name it does not have. False when nothing was drawn.
	bool DrawLineStyle(IRenderer& renderer, const LineStyleLibrary& lineStyles, const std::string& name,
		const std::vector<PointF>& points, const ColorF* shortStretches, SymbolLibrary& symbols, const ColorLookup& color);
}
