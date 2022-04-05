#include "stdafx.h"
#include "ViewingGroups.h"

namespace Portrayal
{
	ViewingGroups::ViewingGroups() 
	{

	}

	ViewingGroups::~ViewingGroups() 
	{
		for (auto i = viewingGroup_v.begin(); i != viewingGroup_v.end(); i++)
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
				ViewingGroup* viewing = new ViewingGroup();
				viewing->GetContents(instruction);
				viewingGroup_v.push_back(viewing);
				viewingGroup.insert({ viewing ->GetId(), viewing });
				
			}
		}
	}

	void ViewingGroups::AddViewingGroup(ViewingGroup* value) 
	{
		viewingGroup_v.push_back(value);
	}

	void ViewingGroups::SetViewingGroup(std::vector<ViewingGroup*> value)
	{
		viewingGroup_v = value;
	}

	ViewingGroup* ViewingGroups::GetViewingGroup(int index)
	{
		return viewingGroup_v.at(index);
	}

	ViewingGroup* ViewingGroups::GetViewingGroup_id(std::wstring id)
	{
		auto find=  viewingGroup.find(id);
		if (find== viewingGroup.end())
		{
			return nullptr;
		}

		return viewingGroup[id];
	}

	std::vector<ViewingGroup*>* ViewingGroups::GetViewingGroup()
	{
		return &viewingGroup_v;
	}


}
