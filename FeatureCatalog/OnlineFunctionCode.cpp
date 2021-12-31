#include "stdafx.h"
#include "OnlineFunctionCode.h"

OnlineFunctionCode::OnlineFunctionCode()
{
	InsertValue(L"download");
	InsertValue(L"information");
	InsertValue(L"offlineAccess");
	InsertValue(L"order");
	InsertValue(L"search");
}

OnlineFunctionCode::~OnlineFunctionCode()
{

}

void OnlineFunctionCode::GetContents(pugi::xml_node& node)
{
	SetValueString(pugi::as_wide(node.child_value()));
}

