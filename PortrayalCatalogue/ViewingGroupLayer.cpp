#include "stdafx.h"
#include "ViewingGroupLayer.h"
#include "ViewingGroup.h"

namespace Portrayal
{
	ViewingGroupLayer::ViewingGroupLayer()
	{

	}

	ViewingGroupLayer::~ViewingGroupLayer()
	{

	}

	void ViewingGroupLayer::GetContents(pugi::xml_node& node)
	{
		auto idNode=node.first_attribute();
		if (idNode!= nullptr)
		{
			auto idName =idNode.name();
			if (!strcmp(idName,"id")) 
			{
				SetId(pugi::as_wide(idNode.value()));
			}
		}
	
		
		for (auto instruction= node.first_child(); instruction; instruction= instruction.next_sibling()) 
		{
			auto instructionName = instruction.name();
			if (strcmp(instructionName,"description")) 
			{

				S100_Description* desc = new S100_Description();
				desc->GetContents(instruction);
				AddDescription(desc);
			}
			
			else if (!strcmp(instructionName,"viewingGroup")) 
			{

				ViewingGroup* viewingGroupNode = new ViewingGroup();
				viewingGroupNode->GetContents(instruction);
				viewingGroup.insert({1,viewingGroupNode});
			}

		}
	}

	void ViewingGroupLayer::AddViewingGroup(int key, ViewingGroup* value)
	{
		viewingGroup[key] = value;
	}

	void ViewingGroupLayer::SetViewingGroup(std::unordered_map<int, ViewingGroup*> value)
	{
		viewingGroup = value;
	}

	ViewingGroup* ViewingGroupLayer::GetViewingGroup(int value)
	{
		if(HasViewingGroup(value)==true)
		{
			return viewingGroup[value];
		}
		else
		{
			return false;
		}

	}

	bool ViewingGroupLayer::HasViewingGroup(int value) 
	{
		auto isView = viewingGroup.find(value);
		if (isView!= viewingGroup.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}