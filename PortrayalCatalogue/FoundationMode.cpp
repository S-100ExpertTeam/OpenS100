#include "stdafx.h"
#include "FoundationMode.h"
#include "ViewingGroup.h"
#include "ViewingGroups.h"

namespace Portrayal
{
	FoundationMode::FoundationMode()
	{

	}

	FoundationMode::~FoundationMode()
	{

	}

	void FoundationMode::GetContents(pugi::xml_node& node,ViewingGroups* view)
	{
	
		for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			auto instructionName = instruction.name();
			if (!strcmp(instructionName, "viewingGroup"))
			{
				auto key = pugi::as_wide( instruction.child_value());
				
				auto Viewing  = view->GetViewingGroup_id(key);
		
				viewingGroup.insert({ key,Viewing });
				viewingGroup_v.push_back(Viewing);
			}
		}
	}

	void FoundationMode::SetViewingGroup(std::wstring key, ViewingGroup* value)
	{
		viewingGroup.insert({ key,value });
	}

	ViewingGroup* FoundationMode::GetViewingGroup(std::wstring key)
	{
		return viewingGroup[key];
	}

	bool FoundationMode::HasVewingGroup(std::wstring key) 
	{
		auto isviewing = viewingGroup.find(key);
		if (isviewing!= viewingGroup.end())
		{
			return true;
		}
		return false;
	}

	std::vector<ViewingGroup*>* FoundationMode::GetViewingGroupVector()
	{
		return &viewingGroup_v;
	}
}



