#pragma once
// A symbol of a portrayal catalogue as shapes, with no backend in it, and the drawing of it on an
// IRenderer (Documents/TechDocs/2026-11-19-r6-skia-backend-design.md, 3.2).
//
// The symbols of a catalogue are SVG files of a narrow kind (S-100 Part 9: SVG Tiny, colours as
// CSS classes that name colour tokens). What is read of a file:
//   path    the commands M, L, H, V and Z, absolute or relative. A path with any other command
//           (a curve, an arc) is read up to it and counted in `unsupported`.
//   rect    the box of the symbol (class "svgBox layout"), or a rectangle drawn like a path.
//           Other layout rectangles are not drawn.
//   circle  the pivot (class "pivotPoint layout", not drawn) or a circle.
// Only the children of <svg> are read: nothing inside <g>, no polyline, polygon or text.
// The colour of a shape is the last word of its class: "fCHBLK" fills with CHBLK, any other
// first letter strokes ("sl f0 sCHBLK" strokes with CHBLK). A stroke is as wide as stroke-width
// says, in millimetres, on the screen: a symbol drawn larger keeps the width of its lines.
#include "DisplayListPainter.h"
#include "IRenderer.h"

#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <vector>

class PortrayalCatalogue;

namespace s100::render
{
	struct SymbolShape
	{
		// The lines of the shape, in millimetres about the pivot of the symbol; empty for a circle.
		std::vector<PathFigure> figures;

		// A circle, when it has a radius.
		PointF center;
		float radius = 0.0f;

		bool fill = false;           // filled; otherwise stroked
		std::string colorToken;
		float strokeWidth = 0.0f;    // millimetres
		float opacity = 1.0f;
	};

	struct SymbolPicture
	{
		std::string name;            // the file name without its extension
		std::vector<SymbolShape> shapes;

		// The box of the symbol (svgBox), in millimetres about the pivot.
		float x = 0.0f;
		float y = 0.0f;
		float width = 0.0f;
		float height = 0.0f;

		// Paths cut short at a command that is not read.
		int unsupported = 0;
	};

	// Reads a symbol file. False if it is not an SVG file.
	bool ReadSymbolPicture(const std::filesystem::path& path, SymbolPicture& picture);

	// The symbols of a catalogue by name, read from their files when first asked for.
	class SymbolLibrary
	{
	public:
		explicit SymbolLibrary(PortrayalCatalogue& catalogue);

		// Nothing for a name the catalogue does not have, or a file that cannot be read.
		const SymbolPicture* Find(const std::string& name);

		size_t Size() const
		{
			return files.size();
		}

	private:
		std::map<std::string, std::filesystem::path> files;
		std::map<std::string, std::unique_ptr<SymbolPicture>> pictures;   // null: tried and failed
	};

	// How wide a stroke of a symbol is on the screen, in device pixels (96 to an inch).
	float SymbolStrokePixels(float millimetres);

	// Draws a symbol picture with its pivot at a point of the surface (device pixels), turned
	// clockwise, at so many pixels to a millimetre. Sets the transform of the renderer and
	// leaves it set. The strokes are as wide as the symbol says whatever the scale, times
	// `strokeScale`: a symbol that is sized by a value (the arrow of a current) gets thinner
	// lines as it gets smaller.
	void DrawSymbolPicture(IRenderer& renderer, const SymbolPicture& picture, PointF at, float rotationDegrees, float scale,
		const ColorLookup& color, float strokeScale = 1.0f);

	// Draws the symbol of a name as a display does: the question mark of the catalogue
	// (QUESMRK1) for a name it does not have, and no information symbol (INFORM01) when those
	// are turned off. False when nothing was drawn.
	bool DrawSymbol(IRenderer& renderer, SymbolLibrary& symbols, const std::string& name, PointF at, float rotationDegrees,
		float scale, const ColorLookup& color);
}
