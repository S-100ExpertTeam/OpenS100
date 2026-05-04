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
			name = attri.value();
		}
		else if (!strcmp(attri.name(), "css"))
		{
			css = attri.value();
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
				delete item;
			}
		}
	}
}

void S100_Palette::SetName(const std::string& value)  { name = value; }
void S100_Palette::SetName(const std::wstring& value) { name = toUtf8(value); }
std::string  S100_Palette::GetName()  { return name; }
std::wstring S100_Palette::GetNameW() { return toWide(name); }

void S100_Palette::SetCss(const std::string& value)  { css = value; }
void S100_Palette::SetCss(const std::wstring& value) { css = toUtf8(value); }
std::string  S100_Palette::GetCss()  { return css; }
std::wstring S100_Palette::GetCssW() { return toWide(css); }

void S100_Palette::AddItem(const std::string& key, S100_Item* value)
{
	items.insert({ key, value });
}

void S100_Palette::AddItem(const std::wstring& key, S100_Item* value)
{
	items.insert({ toUtf8(key), value });
}

void S100_Palette::SetItem(std::unordered_map<std::string, S100_Item*> value)
{
	items = value;
}

S100_Item* S100_Palette::GetItem(const std::string& key)
{
	if (HasItems(key))
	{
		return items[key];
	}
	return nullptr;
}

S100_Item* S100_Palette::GetItem(const std::wstring& key)
{
	return GetItem(toUtf8(key));
}

std::unordered_map<std::string, S100_Item*> S100_Palette::GetItem()
{
	return items;
}

bool S100_Palette::HasItems(const std::string& key)
{
	return items.find(key) != items.end();
}

bool S100_Palette::HasItems(const std::wstring& key)
{
	return HasItems(toUtf8(key));
}
