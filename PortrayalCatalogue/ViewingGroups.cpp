#include "stdafx.h"
#include "ViewingGroups.h"

namespace Portrayal
{
	ViewingGroups::ViewingGroups() 
	{

	}

	ViewingGroups::~ViewingGroups() 
	{
		for (auto i = viewingGroup.begin(); i != viewingGroup.end(); i++)
		{
			delete *i;
		}
	}

	void ViewingGroups::GetContents(pugi::xml_node& node)
	{
		for (pugi::xml_node instruction =node.first_child(); instruction; instruction= instruction.next_sibling())
		{
		
			auto instructionName = instruction.name();
			if (!strcmp(instructionName,"viewingGroup")) 
			{
				viewingGroup.push_back(new ViewingGroup());
				viewingGroup.back()->GetContents(instruction);
			}
		}
	}

	void ViewingGroups::AddViewingGroup(ViewingGroup* value) 
	{
		viewingGroup.push_back(value);
	}

	void ViewingGroups::SetViewingGroup(std::vector<ViewingGroup*> value)
	{
		viewingGroup = value;
	}

	ViewingGroup* ViewingGroups::GetViewingGroup(int index)
	{
		return viewingGroup.at(index);
	}

	std::vector<ViewingGroup*> ViewingGroups::GetViewingGroup()
	{
		return viewingGroup;
	}
}
