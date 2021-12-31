#include "stdafx.h"
#include "DisplayPlanes.h"

namespace Portrayal
{
	DisplayPlanes::DisplayPlanes()
	{

	}

	DisplayPlanes::~DisplayPlanes()
	{

	}

	int DisplayPlanes::GetCountOfDisplayMode()
	{
		return (int)displayMode.size();
	}

	DisplayPlane* DisplayPlanes::GetDisplayMode(int index)
	{
		return displayMode.at(index);
	}

	std::vector<DisplayPlane*> DisplayPlanes::GetDisplayMode()
	{
		return displayMode;
	}

	void DisplayPlanes::SetDisplayMode(DisplayPlane* value) 
	{
		displayMode.push_back(value);
	}

	void DisplayPlanes::SetDisplayMode(std::vector<DisplayPlane*> value)
	{
		displayMode = value;
	}
}