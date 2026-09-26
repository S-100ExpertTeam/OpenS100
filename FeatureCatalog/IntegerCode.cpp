#include "stdafx.h"
#include "IntegerCode.h"

#include "..\\LatLonUtility\\cpp_util.h"

IntegerCode::IntegerCode()
{

}

IntegerCode::~IntegerCode()
{

}

void IntegerCode::GetContents(pugi::xml_node& node)
{
	SetValueInteger(cpp_util::stoi(node.child_value()));
}