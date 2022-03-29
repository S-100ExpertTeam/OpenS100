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

bool S100_ColorProfile::ReadFile(std::wstring path) 
{
	pugi::xml_document xmldoc;
	pugi::xml_parse_result result = xmldoc.load_file(path.c_str());

	pugi::xml_node firstchildNode= xmldoc.first_child();
	const pugi::char_t* colorProfileName = firstchildNode.name();
	
	if (!strcmp(colorProfileName,"colorProfile"))
	{
		GetContents(firstchildNode);
	}
	return true;
}

void S100_ColorProfile::GetContents(pugi::xml_node Node) 
{
	for (auto instruction= Node.first_child(); instruction; instruction=instruction.next_sibling())
	{
		auto instructionName = instruction.name();

		if (!strcmp(instructionName,"colors")) 
		{
			for (auto child=instruction.first_child(); child; child=child.next_sibling())
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
void S100_ColorProfile::GetContentsInfo(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}

	auto idvalue = node.attribute("id").value(); //Bring the attribute value.
	if (idvalue)
	{
		id = pugi::as_wide(idvalue);
	}

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "description"))
		{
			description.GetContents(instruction);
		}

		else if (!strcmp(instructionName, "fileName"))
		{
			fileName = pugi::as_wide(instruction.child_value());
		}

		else if (!strcmp(instructionName, "fileType"))
		{
			fileType = pugi::as_wide(instruction.child_value());
		}

		else if (!strcmp(instructionName, "fileFormat"))
		{
			fileFormat = pugi::as_wide(instruction.child_value());
		}
	}
}

//============================Property====================================//

void S100_ColorProfile::Setid(std::wstring& value)
{
	id = value;
}

std::wstring S100_ColorProfile::Getid() 
{
	return id;
}

void S100_ColorProfile::Setdescription(S100_Description value)
{
	description = value;
}

S100_Description S100_ColorProfile::Getdescription()
{
	return description;
}

void S100_ColorProfile::SetfileName(std::wstring& value)
{
	fileName = value;
}

std::wstring S100_ColorProfile::GetfileName() 
{
	return fileName;
}

void S100_ColorProfile::SetfileType(std::wstring& value )
{
	fileType = value;
}

std::wstring S100_ColorProfile::GetfileType()
{
	return fileType;
}

void S100_ColorProfile::SetfileFormat(std::wstring& value)
{
	fileFormat = value;
}

std::wstring S100_ColorProfile::GetfileFormat() 
{
	return fileFormat;
}

void S100_ColorProfile::Setcolors(std::wstring key, S100_Color* value) 
{
	colors.insert({key,value});
}

std::unordered_map<std::wstring, S100_Color*> S100_ColorProfile::Getcolors() 
{
	return colors;
}

void S100_ColorProfile::Setpaletters(std::wstring key, S100_Palette* value)
{
	palettes.insert({key,value});
}

std::unordered_map<std::wstring, S100_Palette*> S100_ColorProfile::Getpalettes()
{
	return palettes;
}


#pragma warning(disable:4244)
D2D1_COLOR_F S100_ColorProfile::GetD2Color(std::wstring paletteName, std::wstring token)
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


COLORREF S100_ColorProfile::GetRGBRef(std::wstring paletteName, std::wstring token)
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