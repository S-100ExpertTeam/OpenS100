#pragma once
// What the display keeps for a portrayal catalogue: the store the catalogue lists its resource
// files in, and the colours of its palette in use.
//
// A PortrayalCatalogue is catalogue data. The pictures of its symbols, line styles and area
// fills are read by the render code when they are first drawn (SymbolPicture, LineStylePicture,
// PatternTile), so the store only remembers which symbols there are.
#include "DrawTypes.h"

#include <memory>
#include <string>

class PortrayalCatalogue;

namespace Portrayal
{
	class ResourceStore;
}

namespace GISLibrary
{
	namespace PortrayalRender
	{
		// The store GISLibrary gives every portrayal catalogue it opens.
		std::unique_ptr<Portrayal::ResourceStore> NewResourceStore();

		// The colour of a token in the current palette of a catalogue. The instruction model keeps
		// tokens only, so this is asked each time something is drawn. Opaque black when the
		// catalogue has no colour profile or the palette does not know the token.
		ColorF Color(PortrayalCatalogue* pc, const std::wstring& token);

		// Another colour palette (Day/Dusk/Night ...) for a catalogue.
		void ChangePalette(PortrayalCatalogue* pc, const std::string& paletteName);
		void ChangePalette(PortrayalCatalogue* pc, const std::wstring& paletteName);
	}
}
