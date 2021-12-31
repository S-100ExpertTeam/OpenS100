#include "stdafx.h"
#include "RoleType.h"

RoleType::RoleType()
{
	InsertValue(L"association");
	InsertValue(L"aggregation");
	InsertValue(L"composition");
}

RoleType::~RoleType()
{

}

void RoleType::GetContents(pugi::xml_node& node)
{
	SetValueString(pugi::as_wide(node.child_value()));
}