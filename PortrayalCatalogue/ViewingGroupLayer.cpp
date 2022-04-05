#include "stdafx.h"
#include "ViewingGroupLayer.h"
#include "ViewingGroup.h"

namespace Portrayal
{
	class ViewingGroup;

	ViewingGroupLayer::ViewingGroupLayer()
	{

	}

	ViewingGroupLayer::~ViewingGroupLayer()
	{
		for (auto i = viewingGroup_v.begin(); i != viewingGroup_v.end(); i++)
		{
			delete *i;
		}
	}

	void ViewingGroupLayer::GetContents(pugi::xml_node& node)
	{
		auto ischild = node.child_value();//displayMode안에 있는 viewingGroupLayer 일 경우
		if (strcmp(ischild, ""))
		{
			value = pugi::as_wide(node.child_value());
			return;
		}


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
			
			if (!strcmp(instructionName,"description")) 
			{
				S100_Description* desc = new S100_Description();
				desc->GetContents(instruction);
				AddDescription(desc);
			}
			
			else if (!strcmp(instructionName,"viewingGroup")) 
			{
				ViewingGroup* viewingGroupNode = new ViewingGroup();
				viewingGroupNode->GetContents(instruction);
				viewingGroup_v.push_back(viewingGroupNode);

				viewingGroup.insert({ viewingGroupNode->GetId(),viewingGroupNode});
			}

		}
	}

	void ViewingGroupLayer::AddViewingGroup(std::wstring key, ViewingGroup* value)
	{
		viewingGroup[key] = value;
	}

	void ViewingGroupLayer::SetViewingGroup(std::unordered_map<std::wstring , ViewingGroup*> value)
	{
		viewingGroup = value;
	}

	ViewingGroup* ViewingGroupLayer::GetViewingGroup(std::wstring key)
	{
		if(HasViewingGroup(key)==true)
		{
			return viewingGroup[key];
		}
		else
		{
			return false;
		}

	}

	bool ViewingGroupLayer::HasViewingGroup(std::wstring key)
	{
		auto isView = viewingGroup.find(key);
		if (isView!= viewingGroup.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	std::vector<ViewingGroup*>* ViewingGroupLayer::GetViewingGroupVector()
	{
		return &viewingGroup_v;
	}

	std::wstring ViewingGroupLayer::GetValue()
	{
		return value;
	}
}