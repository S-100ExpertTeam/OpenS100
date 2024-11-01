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

		if (!strcmp(instructionName,"token"))
		{
			token = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "transparency"))
		{
			transparency = pugi::as_wide(instruction.child_value());
		}
	}

	token = pugi::as_wide(node.child_value());
}

void S100_Foreground::SetToken(std::wstring& value)
{
	token = value;
}

std::wstring S100_Foreground::GetToken() 
{
	return token;
}

void S100_Foreground::SetTransparency(std::wstring& value)
{
	transparency = value;
}

std::wstring S100_Foreground::GetTransparency() 
{
	return transparency;
}

void S100_Foreground::fromDrawingCommand(std::string_view drawingCommand)
{
    // Parse the drawing command
    std::string token;
    std::string transparency;

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

    // Set the parsed values
    SetToken(std::wstring(token.begin(), token.end()));
    if (!transparency.empty())
    {
        SetTransparency(std::wstring(transparency.begin(), transparency.end()));
    }
}