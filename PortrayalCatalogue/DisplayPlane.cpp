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

		description = new S100_Description();
		description->GetContents(node);
	}
}