#include "stdafx.h"
#include "RoleType.h"

RoleType::RoleType()
{
	InsertValue("association");
	InsertValue("aggregation");
	InsertValue("composition");
}

RoleType::~RoleType()
{

}

void RoleType::GetContents(pugi::xml_node& node)
{
	SetValueString(node.child_value());
}