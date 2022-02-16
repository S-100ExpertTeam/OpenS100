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
			useValueOf = pugi::as_wide(attri.value());
			break;
		}
	}
}

void S100_Text::SetValue(std::wstring& _value)
{
	value = _value;
}

void S100_Text::SetUseValueOf(std::wstring& value)
{
	useValueOf = value;
}

std::wstring S100_Text::GetValue() 
{
	return value;
}

std::wstring S100_Text::GetUseValueOf() 
{
	return useValueOf;
}