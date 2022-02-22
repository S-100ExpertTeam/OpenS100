#include "stdafx.h"
#include "S100_Rotation.h"

S100_Rotation::S100_Rotation()
{

}

S100_Rotation::~S100_Rotation()
{

}

void S100_Rotation::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}

	auto Attribute = node.attribute("useValueOf"); //save atttribute
	if (Attribute)
	{
		value = pugi::as_wide(Attribute.value());
	}


	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "color")) 
		{
		}
	}

}

void S100_Rotation::SetUseValueOf(std::wstring& value)
{
	useValueOf = value;
}

void S100_Rotation::SetValue(std::wstring& v)
{
	value = v;
}

std::wstring S100_Rotation::GetUseValueOf() 
{
	return useValueOf;
}

std::wstring S100_Rotation::GetValue() 
{
	return value;
}