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
	transparency = attribute_transparency.as_string();

	token = node.child_value();

	for (auto colorNode = node.first_attribute(); colorNode; colorNode = colorNode.next_attribute())
	{
		auto colorNodeName = colorNode.name();
		if (!strcmp(colorNodeName, "token"))
		{
			token = colorNode.value();
		}
		else if (!strcmp(colorNodeName, "name"))
		{
			name = colorNode.value();
		}
	}

	for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName, "description"))
		{
			description = instruction.child_value();
		}
	}
}

void S100_Color::SetToken(const std::string& value)  { token = value; }
void S100_Color::SetToken(const std::wstring& value) { token = toUtf8(value); }
std::string  S100_Color::GetToken()  { return token; }
std::wstring S100_Color::GetTokenW() { return toWide(token); }

void S100_Color::SetName(const std::string& value)  { name = value; }
void S100_Color::SetName(const std::wstring& value) { name = toUtf8(value); }
std::string  S100_Color::GetName()  { return name; }
std::wstring S100_Color::GetNameW() { return toWide(name); }

void S100_Color::SetDescription(const std::string& value)  { description = value; }
void S100_Color::SetDescription(const std::wstring& value) { description = toUtf8(value); }
std::string  S100_Color::GetDescription()  { return description; }
std::wstring S100_Color::GetDescriptionW() { return toWide(description); }

void S100_Color::SetTransparency(const std::string& value)  { transparency = value; }
void S100_Color::SetTransparency(const std::wstring& value) { transparency = toUtf8(value); }
std::string  S100_Color::GetTransparency()  { return transparency; }
std::wstring S100_Color::GetTransparencyW() { return toWide(transparency); }
