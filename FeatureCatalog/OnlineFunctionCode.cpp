#include "stdafx.h"
#include "OnlineFunctionCode.h"

OnlineFunctionCode::OnlineFunctionCode()
{
	InsertValue("download");
	InsertValue("information");
	InsertValue("offlineAccess");
	InsertValue("order");
	InsertValue("search");
}

OnlineFunctionCode::~OnlineFunctionCode()
{

}

void OnlineFunctionCode::GetContents(pugi::xml_node& node)
{
	SetValueString(node.child_value());
}

