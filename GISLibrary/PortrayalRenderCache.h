#pragma once
// Direct2D caches for a portrayal catalogue (render side).
//
// A PortrayalCatalogue is catalogue data. Drawing it needs SVG symbols turned into Direct2D
// geometry and area/line patterns rasterised into bitmaps. Those caches are built here, by
// the renderer, instead of inside CatalogManager, so catalogue management stays free of
// Direct2D. Step 2-3 of Documents/TechDocs/2026-11-08-phase1-core-decoupling.md.
#include <string>

class PortrayalCatalogue;

namespace GISLibrary
{
	class D2D1Resources;

	namespace PortrayalRender
	{
		// Build (or rebuild) the render caches of a catalogue. No-op when pc or d2 is null.
		void Prepare(PortrayalCatalogue* pc, D2D1Resources* d2);

		// Re-colour the caches for another colour palette (Day/Dusk/Night ...).
		void ChangePalette(PortrayalCatalogue* pc, const std::string& paletteName, D2D1Resources* d2);
		void ChangePalette(PortrayalCatalogue* pc, const std::wstring& paletteName, D2D1Resources* d2);
	}
}
