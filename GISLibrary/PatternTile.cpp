#include "stdafx.h"
#include "PatternTile.h"

#include "SymbolPicture.h"

#include "../GeoMetryLibrary/ENCCommon.h"
#include "../PortrayalCatalogue/ExternalFile.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"
#include "../PortrayalCatalogue/S100_AreaFills.h"

#include <pugixml.hpp>

#include <cmath>
#include <cstring>
#include <fstream>

namespace
{
	// By name: the precompiled header of the viewer build brings in another PointF.
	using s100::render::PointF;

	std::string Lower(std::string text)
	{
		for (char& letter : text)
		{
			if (letter >= 'A' && letter <= 'Z')
			{
				letter = static_cast<char>(letter - 'A' + 'a');
			}
		}
		return text;
	}

	// The value of name="..." in the text of a processing instruction.
	std::string ValueIn(const std::string& text, const std::string& name)
	{
		const std::string opening = name + "=\"";
		const size_t start = text.find(opening);
		if (start == std::string::npos)
		{
			return {};
		}
		const size_t end = text.find('"', start + opening.size());
		return end == std::string::npos ? std::string{} : text.substr(start + opening.size(), end - start - opening.size());
	}

	PointF Vector(const pugi::xml_node& node)
	{
		return { node.child("x").text().as_float(), node.child("y").text().as_float() };
	}
}

namespace s100::render
{
	bool ReadAreaFill(const std::filesystem::path& path, AreaFillDefinition& areaFill)
	{
		// Read by the standard library, which takes the path as it is on every platform.
		std::ifstream file(path, std::ios::binary);
		if (!file)
		{
			return false;
		}
		pugi::xml_document document;
		if (!document.load(file, pugi::parse_full))
		{
			return false;
		}

		areaFill = AreaFillDefinition{};
		areaFill.name = path.stem().string();
		bool found = false;
		for (pugi::xml_node node = document.first_child(); node; node = node.next_sibling())
		{
			const std::string name = node.name();
			if (name == "S100Meta")
			{
				// <?S100Meta name="DIAMOND1" exposition="..."?>
				const std::string given = ValueIn(node.value(), "name");
				if (!given.empty())
				{
					areaFill.name = given;
				}
			}
			else if (name.find("symbolFill") != std::string::npos)   // af:symbolFill
			{
				found = true;
				for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
				{
					if (!std::strcmp(child.name(), "symbol"))
					{
						areaFill.symbol = child.attribute("reference").value();
						areaFill.offset = Vector(child.child("offset"));
					}
					else if (!std::strcmp(child.name(), "v1"))
					{
						areaFill.v1 = Vector(child);
					}
					else if (!std::strcmp(child.name(), "v2"))
					{
						areaFill.v2 = Vector(child);
					}
				}
			}
		}
		return found;
	}

	AreaFillLibrary::AreaFillLibrary(PortrayalCatalogue& catalogue)
	{
		const auto directory = std::filesystem::path(catalogue.GetRootPathW()) / L"AreaFills";
		if (auto files = catalogue.GetAreaFills()->GetAreaFillFilesVecter())
		{
			for (auto file : *files)
			{
				AreaFillDefinition areaFill;
				if (ReadAreaFill(directory / file->GetFileNameW(), areaFill))
				{
					areaFills.emplace(Lower(areaFill.name), std::move(areaFill));   // the first of a name stays
				}
			}
		}
	}

	const AreaFillDefinition* AreaFillLibrary::Find(const std::string& name) const
	{
		const auto found = areaFills.find(Lower(name));
		return found == areaFills.end() ? nullptr : &found->second;
	}

	PatternTile MakePatternTile(const std::string& symbol, PointF v1, PointF v2, PointF offset, float scale)
	{
		PatternTile tile;
		tile.symbol = symbol;
		tile.scale = scale;

		const bool square = v1.y < 0.0001f && v2.x < 0.0001f;
		const float width = v1.x;
		const float height = square ? v2.y : v2.y * 2.0f;

		// Whole pixels, cut as the viewer has always cut them.
		tile.width = width * scale > 0.0f ? std::floor(width * scale) : 0.0f;
		tile.height = height * scale > 0.0f ? std::floor(height * scale) : 0.0f;

		const auto place = [&](float x, float y) {
			tile.at.push_back({ (x + offset.x) * scale, (y + offset.y) * scale });
		};
		place(0.0f, 0.0f);
		place(width, 0.0f);
		if (square)
		{
			place(width, height);
			place(0.0f, height);
		}
		else
		{
			place(width / 2.0f, v2.y);
			place(0.0f, height);
			place(width, height);
		}
		return tile;
	}

	PatternTile TileOf(const Fill& fill, const AreaFillLibrary& areaFills)
	{
		const float scale = ENCCommon::DISPLAY_SYMBOL_SCALE;
		switch (fill.kind)
		{
		case Fill::Kind::AreaFill:
		{
			const AreaFillDefinition* areaFill = areaFills.Find(fill.reference);
			if (!areaFill && !fill.fallback.empty())
			{
				areaFill = areaFills.Find(fill.fallback);
			}
			if (areaFill)
			{
				return MakePatternTile(areaFill->symbol, areaFill->v1, areaFill->v2, areaFill->offset, scale);
			}
			break;
		}
		case Fill::Kind::SymbolTiles:
			return MakePatternTile(fill.symbol, fill.v1, fill.v2, fill.offset, scale);
		default:
			break;
		}
		return {};
	}

	bool DrawPatternTile(IRenderer& renderer, const PatternTile& tile, SymbolLibrary& symbols, const ColorLookup& color)
	{
		const SymbolPicture* picture = symbols.Find(tile.symbol);
		if (!picture || tile.Empty())
		{
			return false;
		}
		for (const PointF& at : tile.at)
		{
			DrawSymbolPicture(renderer, *picture, at, 0.0f, tile.scale, color);
		}
		return true;
	}
}
