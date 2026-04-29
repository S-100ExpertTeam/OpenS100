#include "stdafx.h"
#include "StringCode.h"

StringCode::StringCode()
{

}

StringCode::~StringCode()
{

}

void StringCode::GetContents(pugi::xml_node& node)
{
	SetValueString(node.child_value());
}