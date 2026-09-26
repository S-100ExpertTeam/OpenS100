#include "stdafx.h"
#include "S100_Foreground.h"

S100_Foreground::S100_Foreground()
{

}

S100_Foreground::~S100_Foreground()
{

}

void S100_Foreground::GetContents(pugi::xml_node node)
{
	if (node == nullptr)
	{
		return;
	}

	for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		auto instructionName = instruction.name();

		if (!strcmp(instructionName, "token"))
		{
			token = instruction.child_value();
		}
		else if (!strcmp(instructionName, "transparency"))
		{
			transparency = instruction.child_value();
		}
	}

	token = node.child_value();
}

void S100_Foreground::SetToken(const std::string& value)  { token = value; }
void S100_Foreground::SetToken(const std::wstring& value) { token = toUtf8(value); }
std::string  S100_Foreground::GetToken()  { return token; }
std::wstring S100_Foreground::GetTokenW() { return toWide(token); }

void S100_Foreground::SetTransparency(const std::string& value)  { transparency = value; }
void S100_Foreground::SetTransparency(const std::wstring& value) { transparency = toUtf8(value); }
std::string  S100_Foreground::GetTransparency()  { return transparency; }
std::wstring S100_Foreground::GetTransparencyW() { return toWide(transparency); }

void S100_Foreground::fromDrawingCommand(std::string_view drawingCommand)
{
	size_t commaPos = drawingCommand.find(',');
	if (commaPos != std::string_view::npos)
	{
		token = drawingCommand.substr(0, commaPos);
		transparency = drawingCommand.substr(commaPos + 1);
	}
	else
	{
		token = drawingCommand;
	}
}
