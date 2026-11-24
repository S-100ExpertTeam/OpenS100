#include "stdafx.h"
#include "PortrayalRenderCache.h"

#include "ColorPalette.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"

#include <filesystem>
#include <set>

namespace
{
	// The symbols a catalogue lists, and the colours of its palette in use (read from the
	// colour profile of the catalogue when first asked for, and again when the palette changes).
	class DisplayStore final : public Portrayal::ResourceStore
	{
	public:
		bool AddColorProfile(const std::wstring&, const std::wstring&) override
		{
			read = false;
			return true;
		}

		bool AddSymbol(const std::wstring& path) override
		{
			symbols.insert(std::filesystem::path(path).stem().wstring());
			return true;
		}

		bool AddLineStyle(const std::wstring&) override
		{
			return true;
		}

		bool AddAreaFill(const std::wstring&) override
		{
			return true;
		}

		bool HasSymbol(const std::wstring& name) override
		{
			return symbols.contains(name);
		}

		void ChangePalette(const std::wstring&) override
		{
			read = false;
		}

		const s100::render::ColorPalette& Colors(PortrayalCatalogue& catalogue)
		{
			const std::string palette = catalogue.GetCurrentPaletteName();
			if (!read || palette != paletteRead)
			{
				colors = {};
				colors.Read(catalogue, palette);
				paletteRead = palette;
				read = true;
			}
			return colors;
		}

	private:
		std::set<std::wstring> symbols;
		s100::render::ColorPalette colors;
		std::string paletteRead;
		bool read = false;
	};
}

namespace GISLibrary
{
	namespace PortrayalRender
	{
		std::unique_ptr<Portrayal::ResourceStore> NewResourceStore()
		{
			return std::make_unique<DisplayStore>();
		}

		ColorF Color(PortrayalCatalogue* pc, const std::wstring& token)
		{
			// Every store in this library comes from NewResourceStore().
			auto store = pc ? static_cast<DisplayStore*>(pc->GetResources()) : nullptr;
			return store ? store->Colors(*pc).Color(token) : ColorF{};
		}

		void ChangePalette(PortrayalCatalogue* pc, const std::string& paletteName)
		{
			if (pc)
			{
				pc->ChangePalette(paletteName);
			}
		}

		void ChangePalette(PortrayalCatalogue* pc, const std::wstring& paletteName)
		{
			ChangePalette(pc, pugi::as_utf8(paletteName));
		}
	}
}
