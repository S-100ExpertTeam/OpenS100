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
		auto firstNodeName = node.first_child().name();
		if (!strcmp(firstNodeName,"")) 
		{
			value = std::stoi(node.child_value());
			return;
		}

		auto IdAttri = node.attribute("id");
		if (IdAttri!=nullptr)
		{
			auto value =pugi::as_wide( IdAttri.value());
			SetId(value);
		}
		
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
				//auto idAttri = instruction.attribute("id");
				//if (idAttri!=nullptr)
				//{
				//	auto value =pugi::as_wide(idAttri.value());
				//	SetId(value);
				//}

				S100_Description* desc = new S100_Description();
				desc->GetContents(instruction.first_child());
				AddDescription(desc);
			}
		}
	}
}
