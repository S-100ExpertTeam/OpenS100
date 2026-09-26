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
	std::string name;
	std::unordered_map<std::string, libS100Engine::Item> item;

public:
	void SetName(const std::string& value);
	void SetName(const std::wstring& value);
	std::string  GetName();
	std::wstring GetNameW();
	void SetItem(const std::string& key, libS100Engine::Item value);
	void SetItem(const std::wstring& key, libS100Engine::Item value);
	libS100Engine::Item* GetItem(const std::string& key);
	libS100Engine::Item* GetItem(const std::wstring& key);
	bool IsItem(const std::string& key);
	bool IsItem(const std::wstring& key);
};