#include "stdafx.h"
#include "DisplayModes.h"

namespace Portrayal
{
	DisplayModes::DisplayModes()
	{

	}

	DisplayModes::~DisplayModes()
	{

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

	std::vector<DisplayMode*> DisplayModes::GetDisplayMode()
	{
		return displayMode;
	}

	int DisplayModes::GetCountOfDisplayMode() 
	{
		return (int)displayMode.size();
	}
}