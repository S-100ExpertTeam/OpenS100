#include "stdafx.h"
#include "Palette.h"

Palette::Palette()
{
}

Palette::~Palette()
{
}

void Palette::SetName(std::wstring value) 
{
	name = value;
}

std::wstring Palette::GetName()
{
	return name;
}

void Palette::SetItem(std::wstring key, libS100Engine::Item value) 
{
	item.insert({key,value});
}

libS100Engine::Item* Palette::GetItem(std::wstring& key) 
{
	auto find = item.find(key);

	if (find == item.end())
	{
		return nullptr;
	}
	
	return &find->second;
}

bool Palette::IsItem(std::wstring key) 
{
	auto  result = item.find(key);
	if (result==item.end()) 
	{
		return false;
	}
	else 
	{
		return true;
	}
}