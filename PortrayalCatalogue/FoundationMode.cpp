#include "stdafx.h"
#include "FoundationMode.h"
#include "ViewingGroup.h"

namespace Portrayal
{
	FoundationMode::FoundationMode()
	{

	}

	FoundationMode::~FoundationMode()
	{

	}

	void FoundationMode::GetContents(pugi::xml_node& node)
	{
		int index = 0;
		for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			auto instructionName = instruction.name();
			if (!strcmp(instructionName, "viewingGroup"))
			{
				ViewingGroup* value = new ViewingGroup();
				value->GetContents(instruction);
				//viewingGroup.insert({ 1,value });
				viewingGroup.insert({ index,value });
				index++;
			}
		}
	}

	void FoundationMode::SetViewingGroup(int index, ViewingGroup* value)
	{
		viewingGroup.insert({ index,value });
	}

	ViewingGroup* FoundationMode::GetViewingGroup(int value)
	{
		return viewingGroup[value];
	}

	bool FoundationMode::HasVewingGroup(int value) 
	{
		auto isviewing = viewingGroup.find(value);
		if (isviewing!= viewingGroup.end())
		{
			return true;
		}
		return false;
	}
}

