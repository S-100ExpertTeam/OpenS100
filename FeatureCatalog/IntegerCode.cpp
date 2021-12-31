#include "stdafx.h"
#include "IntegerCode.h"

IntegerCode::IntegerCode()
{

}

IntegerCode::~IntegerCode()
{

}

void IntegerCode::GetContents(pugi::xml_node& node)
{
	SetValueInteger(std::stoi(node.child_value()));
}