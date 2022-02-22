#include "stdafx.h"
#include "S100_Color.h"

S100_Color::S100_Color()
{

}

S100_Color::~S100_Color()
{

}

void S100_Color::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}

	for (auto colorNode = node.first_attribute(); colorNode; colorNode = colorNode.next_attribute())
	{
		auto colorNodeName = colorNode.name();
		if (!strcmp(colorNodeName, "token"))
		{
			token = pugi::as_wide(colorNode.value());
		}
		else if (!strcmp(colorNodeName, "name"))
		{
			name = pugi::as_wide(colorNode.value());
		}
	}

	for (auto instruction= node.first_child(); instruction; instruction= instruction.next_sibling()) 
	{
		auto instructionName = instruction.name(); 
		if (!strcmp(instructionName,"description"))
		{
			description = pugi::as_wide(instruction.child_value());
		}
	}
}

void S100_Color::SetToken(std::wstring& value)
{
	token = value;
}

std::wstring S100_Color::GetToken() 
{
	return token;
}

void S100_Color::SetName(std::wstring& value)
{
	name = value;
}

std::wstring S100_Color::GetName() 
{
	return name;
}

void S100_Color::SetDescription(std::wstring& value)
{
	description = value;
}

std::wstring S100_Color::GetDescription()
{
	return description;
}

void S100_Color::SetTransparency(std::wstring& value)
{
	transparency = value;
}

std::wstring S100_Color::GetTransparency()
{
	return transparency;
}