#include "stdafx.h"
#include "ViewingGroup.h"

namespace Portrayal
{
	ViewingGroup::ViewingGroup() 
	{

	}

	ViewingGroup::~ViewingGroup()
	{

	}

	void ViewingGroup::GetContents(pugi::xml_node& node) 
	{
		
		for (auto instruction= node.first_child(); instruction; instruction=instruction.next_sibling())
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
				S100_Description* desc = new S100_Description();
				desc->GetContents(instruction.first_child());
				AddDescription(desc);
			}
		}
	}
}
