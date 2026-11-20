#pragma once
// The tile of a pattern fill, with no backend in it
// (Documents/TechDocs/2026-11-19-r6-skia-backend-design.md, 3.2).
//
// An area is filled with a pattern by repeating a symbol on a lattice: two vectors (v1, v2) say
// where the next symbol is, in millimetres. The catalogue has such fills as files (AreaFills);
// a rule can also give the lattice itself. Either way the pattern is a rectangular tile of
// device pixels with the symbol drawn at some places of it, repeated over the surface.
//
// The lattices of the catalogues are of two kinds, and so are the tiles:
//   square     v1 runs along x and v2 along y: the tile is v1.x by v2.y, a symbol at each corner
//   staggered  every other row is shifted half a step: the tile is v1.x by twice v2.y, a symbol
//              at each corner and one in the middle
// A symbol at a corner is cut by the tile; the four cuts make it whole when the tile repeats.
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

	// An area fill of a catalogue: the file of a symbolFill.
	struct AreaFillDefinition
	{
		std::string name;     // as the file names itself (S100Meta), or the file name
		std::string symbol;
		PointF v1;            // millimetres
		PointF v2;
		PointF offset;        // of the symbol in the tile, millimetres
	};

	// Reads an area fill file. False if it has no symbolFill.
	bool ReadAreaFill(const std::filesystem::path& path, AreaFillDefinition& areaFill);

	// The area fills of a catalogue by name (whatever the case of the letters), read when the
	// library is made: they are few.
	class AreaFillLibrary
	{
	public:
		explicit AreaFillLibrary(PortrayalCatalogue& catalogue);

		const AreaFillDefinition* Find(const std::string& name) const;

		size_t Size() const
		{
			return areaFills.size();
		}

	private:
		std::map<std::string, AreaFillDefinition> areaFills;   // by name in lower case
	};

	struct PatternTile
	{
		std::string symbol;
		// Whole device pixels: the tile repeats with no seam.
		float width = 0.0f;
		float height = 0.0f;
		// Pixels to a millimetre of the symbol.
		float scale = 0.0f;
		// Where the pivot of the symbol goes, in pixels of the tile.
		std::vector<PointF> at;

		bool Empty() const
		{
			return width < 1.0f || height < 1.0f || at.empty();
		}
	};

	// The tile of a lattice at so many pixels to a millimetre.
	PatternTile MakePatternTile(const std::string& symbol, PointF v1, PointF v2, PointF offset, float scale);

	// The tile a pattern fill asks for, at the symbol scale of the display: of the area fill it
	// names (or of its fallback, when the catalogue has none of that name), or of its own
	// lattice. An empty tile when there is nothing to draw.
	PatternTile TileOf(const Fill& fill, const AreaFillLibrary& areaFills);

	// Draws the symbols of a tile on a renderer whose surface is the tile. False when the
	// catalogue does not have the symbol: such a pattern is not drawn at all.
	bool DrawPatternTile(IRenderer& renderer, const PatternTile& tile, SymbolLibrary& symbols, const ColorLookup& color);
}
