#pragma once
// Direct2D caches for a portrayal catalogue (render side).
//
// A PortrayalCatalogue is catalogue data. Drawing it needs SVG symbols turned into Direct2D
// geometry and area/line patterns rasterised into bitmaps. Those caches are built here, by
// the renderer, instead of inside CatalogManager, so catalogue management stays free of
// Direct2D. Step 2-3 of Documents/TechDocs/2026-11-08-phase1-core-decoupling.md.
#include "DrawTypes.h"

#include <memory>
#include <string>

class PortrayalCatalogue;
class S100PCManager;

namespace Portrayal
{
	class ResourceStore;
}

namespace GISLibrary
{
	class D2D1Resources;

	namespace PortrayalRender
	{
		// The store GISLibrary gives every portrayal catalogue it opens.
		std::unique_ptr<Portrayal::ResourceStore> NewResourceStore();

		// That store of a catalogue, with its Direct2D caches. Null when pc is null or was
		// opened without a store.
		S100PCManager* Caches(PortrayalCatalogue* pc);

		// The colour of a token in the current palette of a catalogue. The instruction model keeps
		// tokens only, so this is asked each time something is drawn. Opaque black when the
		// catalogue has no colour profile or the palette does not know the token.
		ColorF Color(PortrayalCatalogue* pc, const std::wstring& token);

		// Build (or rebuild) the render caches of a catalogue. No-op when pc or d2 is null.
		void Prepare(PortrayalCatalogue* pc, D2D1Resources* d2);

		// Re-colour the caches for another colour palette (Day/Dusk/Night ...).
		void ChangePalette(PortrayalCatalogue* pc, const std::string& paletteName, D2D1Resources* d2);
		void ChangePalette(PortrayalCatalogue* pc, const std::wstring& paletteName, D2D1Resources* d2);
	}
}
