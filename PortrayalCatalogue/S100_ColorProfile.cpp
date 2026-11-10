#include "stdafx.h"
#include "S100_ColorProfile.h"

#include <string_view>
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

	// The root is <colorProfile>, or <cp:colorProfile> in profiles written to the S-100 5.x
	// schema (the S-102 PC 3.0.0's): compared by its local name.
	pugi::xml_node root = xmldoc.document_element();
	const std::string_view name = root.name();
	const auto colon = name.find(':');
	const std::string_view localName = colon == std::string_view::npos ? name : name.substr(colon + 1);
	if (localName != "colorProfile")
	{
		return false;
	}
	GetContents(root);
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


