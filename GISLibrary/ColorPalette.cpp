#include "stdafx.h"
#include "ColorPalette.h"

#include "../PortrayalCatalogue/ExternalFile.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"
#include "../PortrayalCatalogue/S100_ColorProfile.h"
#include "../PortrayalCatalogue/S100_ColorProfiles.h"
#include "../PortrayalCatalogue/S100_Item.h"
#include "../PortrayalCatalogue/S100_Palette.h"
#include "../PortrayalCatalogue/S100_SRGB.h"

#include <filesystem>

namespace s100::render
{
	bool ColorPalette::Read(PortrayalCatalogue& catalogue, const std::string& palette)
	{
		colors.clear();

		auto files = catalogue.GetColorProfiles()->GetColorProfilesVector();
		if (!files)
		{
			return false;
		}
		for (auto file : *files)
		{
			const auto path = std::filesystem::path(catalogue.GetRootPathW()) / L"ColorProfiles" / file->GetFileNameW();

			S100_ColorProfile profile;
			if (!profile.ReadFile(path.wstring()))
			{
				continue;
			}
			const auto palettes = profile.Getpalettes();
			const auto found = palettes.find(palette);
			if (found == palettes.end() || !found->second)
			{
				continue;
			}
			for (const auto& [token, item] : found->second->GetItem())
			{
				if (!item || !item->GetSRGB())
				{
					continue;
				}
				auto srgb = item->GetSRGB();
				colors[item->GetTokenW()] = GISLibrary::ColorF{
					static_cast<float>(srgb->GetRed() / 255.0),
					static_cast<float>(srgb->GetGreen() / 255.0),
					static_cast<float>(srgb->GetBlue() / 255.0),
					1.0f };
			}
			return true;
		}
		return false;
	}

	GISLibrary::ColorF ColorPalette::Color(const std::wstring& token) const
	{
		const auto found = colors.find(token);
		return found == colors.end() ? GISLibrary::ColorF{} : found->second;
	}
}
