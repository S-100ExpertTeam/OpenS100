#include "stdafx.h"
#include "Attribute.h"

Attribute::Attribute()
{

}

Attribute::~Attribute()
{

}

void Attribute::GetContents(pugi::xml_node& node)
{
   ((Item*)this)->Item::GetContents(node);
}