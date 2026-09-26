#include "stdafx.h"
#include "Palette.h"

Palette::Palette()
{
}

Palette::~Palette()
{
}

void Palette::SetName(const std::string& value)
{
	name = value;
}

void Palette::SetName(const std::wstring& value)
{
	name = LibMFCUtil::WStringToString(value);
}

std::string Palette::GetName()
{
	return name;
}

std::wstring Palette::GetNameW()
{
	return LibMFCUtil::StringToWString(name);
}

void Palette::SetItem(const std::string& key, libS100Engine::Item value)
{
	item.insert({ key, value });
}

void Palette::SetItem(const std::wstring& key, libS100Engine::Item value)
{
	item.insert({ LibMFCUtil::WStringToString(key), value });
}

libS100Engine::Item* Palette::GetItem(const std::string& key)
{
	auto find = item.find(key);
	if (find == item.end())
	{
		return nullptr;
	}
	return &find->second;
}

libS100Engine::Item* Palette::GetItem(const std::wstring& key)
{
	auto skey = LibMFCUtil::WStringToString(key);
	return GetItem(skey);
}

bool Palette::IsItem(const std::string& key)
{
	return item.find(key) != item.end();
}

bool Palette::IsItem(const std::wstring& key)
{
	return IsItem(LibMFCUtil::WStringToString(key));
}