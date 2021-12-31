#pragma once
#include "S100_Item.h"

#include <unordered_map>
#include <string>

#include "..\\extlibs\pugixml\include\pugixml.hpp"

class S100_Palette
{
public:
	S100_Palette();
	virtual ~S100_Palette();

private:
	std::wstring name;
	std::wstring css;
	std::unordered_map<std::wstring, S100_Item*> items;

public:
	void GetContents(pugi::xml_node Node);

	void SetName(std::wstring& value);
	std::wstring GetName();

	void SetCss(std::wstring& value);
	std::wstring GetCss();

	void AddItem(std::wstring key, S100_Item* value);
	void SetItem(std::unordered_map<std::wstring, S100_Item*> value);

	S100_Item* GetItem(std::wstring key);
	std::unordered_map<std::wstring, S100_Item*> GetItem();
	bool HasItems(std::wstring key);
};