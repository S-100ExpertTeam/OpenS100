#include "stdafx.h"
#include "S100_Text.h"

S100_Text::S100_Text()
{

}

S100_Text::~S100_Text()
{

}

void S100_Text::GetContents(pugi::xml_node node)
{
	if (node == nullptr)
	{
		return;
	}

	for (auto attri = node.first_attribute(); attri; attri = attri.next_attribute())
	{
		auto attriName = attri.name();
		if (!strcmp(attriName, "useValueOf"))
		{
			useValueOf = attri.value();
			break;
		}
	}

	value = node.child_value();
}

void S100_Text::SetValue(const std::string& v)  { value = v; }
void S100_Text::SetValue(const std::wstring& v) { value = toUtf8(v); }
void S100_Text::SetUseValueOf(const std::string& v)  { useValueOf = v; }
void S100_Text::SetUseValueOf(const std::wstring& v) { useValueOf = toUtf8(v); }

std::string  S100_Text::GetValue()      { return value; }
std::wstring S100_Text::GetValueW()     { return toWide(value); }
std::string  S100_Text::GetUseValueOf()  { return useValueOf; }
std::wstring S100_Text::GetUseValueOfW() { return toWide(useValueOf); }
