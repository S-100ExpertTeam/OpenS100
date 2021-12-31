#include "stdafx.h"
#include "ContactInfo.h"

ContactInfo::ContactInfo()
{

}

ContactInfo::~ContactInfo()
{

}

void ContactInfo::GetContents(pugi::xml_node& node) 
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling()) 
	{
		const pugi::char_t *instructionName = instruction.name();
		if (!strcmp(instructionName,"S100CI:address"))
		{
			Address add;
			add.GetContents(instruction);
			SetAddress(add);
		}
		else if (!strcmp(instructionName, "S100CI:onlineResource"))
		{
			OnlineResource online;
			online.GetContents(instruction);
			SetOnlineResource(online);
		}
	}
}

void ContactInfo::SetAddress(Address& value)
{
	address = value;
}

Address& ContactInfo::GetAddress()
{
	return address;
}

void ContactInfo::SetOnlineResource(OnlineResource& value)
{
	onlineResource = value;
}

OnlineResource& ContactInfo::GetOnlineResource()
{
	return onlineResource;
}