#pragma once
// The colours of one palette of a portrayal catalogue, with no backend in it.
//
// Drawing instructions carry colour tokens; a painter asks for the colour of a token each time
// it draws (DisplayListPainter's ColorLookup). This reads the colour profile of a catalogue
// from its file and answers for one palette (Day, Dusk, Night).
#include "DrawTypes.h"

#include <string>
#include <unordered_map>

class PortrayalCatalogue;

namespace s100::render
{
	class ColorPalette
	{
	public:
		// The palette of that name in the first colour profile of the catalogue that has it.
		// False if the catalogue has none: every token is black then.
		bool Read(PortrayalCatalogue& catalogue, const std::string& palette = "Day");

		// The sRGB colour of a token; black for a token the palette does not have.
		GISLibrary::ColorF Color(const std::wstring& token) const;

		size_t Size() const
		{
			return colors.size();
		}

	private:
		std::unordered_map<std::wstring, GISLibrary::ColorF> colors;
	};
}
