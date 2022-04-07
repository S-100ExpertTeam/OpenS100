#include "stdafx.h"
#include "DisplayModes.h"
#include "DisplayMode.h"

namespace Portrayal
{
	DisplayModes::DisplayModes()
	{

	}

	DisplayModes::~DisplayModes()
	{
		for (auto i = displayMode.begin(); i != displayMode.end(); i++)
		{
			delete *i;
		}
	}

	void DisplayModes::SetDisplayMode(DisplayMode* value)
	{
		displayMode.push_back(value);
	}

	void DisplayModes::SetDisplayMode(std::vector<DisplayMode*> value)
	{
		displayMode = value;
	}

	DisplayMode* DisplayModes::GetDisplayMode(int index)
	{
		return displayMode[index];
	}

	std::vector<DisplayMode*>* DisplayModes::GetDisplayModeVector()
	{
		return &displayMode;
	}

	int DisplayModes::GetCountOfDisplayMode() 
	{
		return (int)displayMode.size();
	}

	void DisplayModes::GetContents(pugi::xml_node& node) {

		for (auto instruction= node.first_child(); instruction; instruction=instruction.next_sibling())
		{
			auto instructionName = instruction.name();
			if (!strcmp(instructionName,"displayMode"))
			{
				DisplayMode* mode = new DisplayMode();
				mode->GetContents(instruction);
				displayMode.push_back(mode);
			}
		}
	}
}