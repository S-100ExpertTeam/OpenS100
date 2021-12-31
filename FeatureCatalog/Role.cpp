#include "stdafx.h"
#include "Role.h"

Role::Role()
{

}

Role::~Role()
{

}

void Role::GetContents(pugi::xml_node& node)
{
	this->Item::GetContents(node);
}