#pragma once
#include "S100_Item.h"

#include <unordered_map>
#include <string>

#include <pugixml.hpp>

class S100_Palette
{
public:
	S100_Palette();
	virtual ~S100_Palette();

private:
	std::string name;
	std::string css;
	std::unordered_map<std::string, S100_Item*> items;

public:
	void GetContents(pugi::xml_node Node);

	void SetName(const std::string& value);
	void SetName(const std::wstring& value);
	std::string  GetName();
	std::wstring GetNameW();

	void SetCss(const std::string& value);
	void SetCss(const std::wstring& value);
	std::string  GetCss();
	std::wstring GetCssW();

	void AddItem(const std::string& key, S100_Item* value);
	void AddItem(const std::wstring& key, S100_Item* value);
	void SetItem(std::unordered_map<std::string, S100_Item*> value);

	S100_Item* GetItem(const std::string& key);
	S100_Item* GetItem(const std::wstring& key);
	std::unordered_map<std::string, S100_Item*> GetItem();
	bool HasItems(const std::string& key);
	bool HasItems(const std::wstring& key);
};
