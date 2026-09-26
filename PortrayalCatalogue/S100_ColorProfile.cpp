#include "stdafx.h"
#include "S100_ColorProfile.h"
#include "S100_Palette.h"
#include "S100_Color.h"

S100_ColorProfile::S100_ColorProfile()
{

}

S100_ColorProfile::~S100_ColorProfile()
{
	for (auto i = colors.begin(); i != colors.end(); i++)
	{
		delete i->second;
	}

	for (auto i = palettes.begin(); i != palettes.end(); i++)
	{
		delete i->second;
	}
}

bool S100_ColorProfile::ReadFile(const std::string& path)
{
	pugi::xml_document xmldoc;
	pugi::xml_parse_result result = xmldoc.load_file(path.c_str());

	pugi::xml_node firstchildNode = xmldoc.first_child();
	const pugi::char_t* colorProfileName = firstchildNode.name();

	if (!strcmp(colorProfileName, "colorProfile"))
	{
		GetContents(firstchildNode);
	}
	return true;
}

bool S100_ColorProfile::ReadFile(const std::wstring& path)
{
	return ReadFile(toUtf8(path));
}

void S100_ColorProfile::GetContents(pugi::xml_node Node)
{
	for (auto instruction = Node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		auto instructionName = instruction.name();

		if (!strcmp(instructionName, "colors"))
		{
			for (auto child = instruction.first_child(); child; child = child.next_sibling())
			{
				auto color = new S100_Color();
				color->GetContents(child);
				colors[color->GetToken()] = color;
				colors_v.push_back(color);
			}
		}
		else if (!strcmp(instructionName, "palette"))
		{
			auto palette = new S100_Palette();
			palette->GetContents(instruction);
			palettes[palette->GetName()] = palette;
			palettes_v.push_back(palette);
		}
	}
}

void S100_ColorProfile::Setcolors(const std::string& key, S100_Color* value)  { colors.insert({ key, value }); }
void S100_ColorProfile::Setcolors(const std::wstring& key, S100_Color* value) { colors.insert({ toUtf8(key), value }); }
std::unordered_map<std::string, S100_Color*> S100_ColorProfile::Getcolors()   { return colors; }

void S100_ColorProfile::Setpaletters(const std::string& key, S100_Palette* value)  { palettes.insert({ key, value }); }
void S100_ColorProfile::Setpaletters(const std::wstring& key, S100_Palette* value) { palettes.insert({ toUtf8(key), value }); }
std::unordered_map<std::string, S100_Palette*> S100_ColorProfile::Getpalettes()    { return palettes; }

#pragma warning(disable:4244)
D2D1_COLOR_F S100_ColorProfile::GetD2Color(const std::string& paletteName, const std::string& token)
{
	auto item = palettes.find(paletteName);
	if (item != palettes.end())
	{
		auto color = item->second->GetItem().find(token);

		if (color != item->second->GetItem().end())
		{
			return D2D1::ColorF(
				color->second->GetSRGB()->GetRed() / 255.0,
				color->second->GetSRGB()->GetGreen() / 255.0,
				color->second->GetSRGB()->GetBlue() / 255.0);
		}
	}

	return D2D1::ColorF(0, 0, 0);
}

D2D1_COLOR_F S100_ColorProfile::GetD2Color(const std::wstring& paletteName, const std::wstring& token)
{
	return GetD2Color(toUtf8(paletteName), toUtf8(token));
}

COLORREF S100_ColorProfile::GetRGBRef(const std::string& paletteName, const std::string& token)
{
	auto item = palettes.find(paletteName);
	if (item != palettes.end())
	{
		auto items = item->second->GetItem();
		auto color = items.find(token);

		if (color != items.end())
		{
			return RGB(color->second->GetSRGB()->GetRed(), color->second->GetSRGB()->GetGreen(), color->second->GetSRGB()->GetBlue());
		}
	}

	return RGB(0, 0, 0);
}

COLORREF S100_ColorProfile::GetRGBRef(const std::wstring& paletteName, const std::wstring& token)
{
	return GetRGBRef(toUtf8(paletteName), toUtf8(token));
}
