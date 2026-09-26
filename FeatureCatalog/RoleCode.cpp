#include "stdafx.h"
#include "RoleCode.h"

RoleCode::RoleCode()
{
	InsertValue("resourceProvider");
	InsertValue("custodian");
	InsertValue("owner");
	InsertValue("user");
	InsertValue("distributer");
	InsertValue("originator");
	InsertValue("pointOfContact");
	InsertValue("principalInvestigator");
	InsertValue("processor");
	InsertValue("publisher");
}

RoleCode::~RoleCode()
{

}

void RoleCode::GetContents(pugi::xml_node& node)
{
	SetValueString(node.child_value());
}