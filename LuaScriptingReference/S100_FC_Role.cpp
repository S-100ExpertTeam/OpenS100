#include "stdafx.h"
#include "S100_FC_Role.h"

S100_FC_Role::S100_FC_Role() 
{

}

S100_FC_Role::S100_FC_Role(rapidxml::xml_node<>* node) : S100_FC_Item(node)
{

}

S100_FC_Role::S100_FC_Role(Role* a) : S100_FC_Item((Item*)a)
{

}