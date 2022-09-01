#include "stdafx.h"
#include "S100_Palette.h"

S100_Palette::S100_Palette()
{

}

S100_Palette::~S100_Palette()
{
	for (auto itor = items.begin(); itor != items.end(); itor++)
	{
		delete itor->second;
	}

	items.clear();
}

void S100_Palette::GetContents(pugi::xml_node node)
{
	for (pugi::xml_attribute attri = node.first_attribute(); attri; attri = attri.next_attribute())
	{
		if (!strcmp(attri.name(), "name"))
		{
			name = pugi::as_wide(attri.value());
		}
		else if (!strcmp(attri.name(), "css"))
		{
			css = pugi::as_wide(attri.value());
		}
	}

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "item"))
		{
			auto item = new S100_Item();
			item->GetContents(instruction);

			auto find = items.find(item->GetToken());
			if (find == items.end())
			{
				items[item->GetToken()] = item;
			}
			else
			{
				//OutputDebugString(_T("items already exist\n"));
				delete item;
			}
		}
	}
}

void S100_Palette::SetName(std::wstring& value) 
{
	name = value;
}

std::wstring S100_Palette::GetName() 
{
	return name;
}

void S100_Palette::SetCss(std::wstring& value)
{
	css = value;
}

std::wstring S100_Palette::GetCss() 
{
	return css;
}

void S100_Palette::AddItem(std::wstring key, S100_Item* value) 
{
	items.insert({ key,value });
}

void S100_Palette::SetItem(std::unordered_map<std::wstring, S100_Item*> value) 
{
	items = value;
}

S100_Item* S100_Palette::GetItem(std::wstring key) 
{
	if (HasItems(key)==true)
	{
		return items[key];
	}
	return nullptr;
}

std::unordered_map<std::wstring, S100_Item*> S100_Palette::GetItem()
{
	return items;
}

bool S100_Palette::HasItems(std::wstring key)
{
	auto isitems = items.find(key);
	if (isitems!=items.end()) 
	{
		return true;
	}
	return false;
}