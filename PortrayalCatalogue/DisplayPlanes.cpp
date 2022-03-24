#include "stdafx.h"
#include "DisplayPlanes.h"
#include "DisplayPlane.h"


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

	void DisplayPlanes::GetContents(pugi::xml_node& node) 
	{
		for (auto instruction = node.first_child(); instruction; instruction=instruction.next_sibling())
		{
			auto nodeName = instruction.name();
			if (!strcmp(nodeName,"displayPlane"))
			{
				DisplayPlane* dis = new DisplayPlane();
				dis->GetContents(instruction);
				displayPlanes.push_back(dis);
			}
		}
	}
}