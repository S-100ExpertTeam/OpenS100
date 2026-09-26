#include "stdafx.h"
#include "DateTypeCode.h"

DateTypeCode::DateTypeCode()
{
	InsertValue("creation");
	InsertValue("publication");
	InsertValue("revision");
}

DateTypeCode::~DateTypeCode()
{

}

void DateTypeCode::GetContents(pugi::xml_node& node)
{
	SetValueString(node.child_value());
}