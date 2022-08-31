#pragma once
#include "Item.h"

#include <string>
#include <unordered_map>

class Palette
{
public:
	Palette();
	virtual ~Palette();

private:
	std::wstring name;
	std::unordered_map<std::wstring, libS100Engine::Item> item;

public:
	void SetName(std::wstring value);
	std::wstring GetName();
	void SetItem(std::wstring key, libS100Engine::Item value);
	libS100Engine::Item* GetItem(std::wstring& key);
	//Determine whether there is a key value.
	bool IsItem(std::wstring key);
};