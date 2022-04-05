#include "stdafx.h"
#include "DisplayPlane.h"

namespace Portrayal
{
	DisplayPlane::DisplayPlane()
	{
	}

	DisplayPlane::~DisplayPlane()
	{
	}

	int DisplayPlane::GetOrder()
	{
		return order;
	}

	void DisplayPlane::SetOrder(int value) 
	{
		order = value;
	}

	void DisplayPlane::GetContents(pugi::xml_node& node)
	{
		if (node==nullptr)
		{
			return;
		}

		auto id =pugi::as_wide(node.attribute("id").value());
		SetId(id);

		auto order_value = node.attribute("order").value();
		SetOrder(std::stoi(order_value));

		for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			auto instructionName = instruction.name();
			if (!strcmp(instructionName, "description"))
			{
				S100_Description* desc = new S100_Description();
				desc->GetContents(instruction);
				AddDescription(desc);
			}
		}
	}
}