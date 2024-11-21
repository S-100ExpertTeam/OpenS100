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

	auto attribute_transparency = node.attribute("transparency");
	transparency = pugi::as_wide(attribute_transparency.as_string());

	token = pugi::as_wide(node.child_value());

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

void S100_Color::SetToken(std::string& value)
{
	if (value.empty())
		token = _T("");
	else
	{
		std::wstring Token(value.begin(), value.end());
		SetToken(Token);
	}
}

std::wstring S100_Color::GetToken() 
{
	return token;
}

void S100_Color::SetName(std::wstring& value)
{
	name = value;
}

void S100_Color::SetName(std::string& value)
{
	if (value.empty())
		name = _T("");
	else
	{
		std::wstring Name(value.begin(), value.end());
		SetName(Name);
	}
}

std::wstring S100_Color::GetName() 
{
	return name;
}

void S100_Color::SetDescription(std::wstring& value)
{
	description = value;
}

void S100_Color::SetDescription(std::string& value)
{
	if (value.empty())
		description = _T("");
	else
	{
		std::wstring Description(value.begin(), value.end());
		SetDescription(Description);
	}
}

std::wstring S100_Color::GetDescription()
{
	return description;
}

void S100_Color::SetTransparency(std::wstring& value)
{
	transparency = value;
}

void S100_Color::SetTransparency(std::string& value)
{
	if (value.empty())
		transparency = _T("");
	else
	{
		std::wstring Transparency(value.begin(), value.end());
		SetTransparency(Transparency);
	}
}

std::wstring S100_Color::GetTransparency()
{
	return transparency;
}