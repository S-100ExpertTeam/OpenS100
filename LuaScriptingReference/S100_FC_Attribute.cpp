#include "stdafx.h"
#include "S100_FC_Attribute.h"

#include "..\\FeatureCatalog\\Attribute.h"
#include "..\\FeatureCatalog\\Item.h"

S100_FC_Attribute::S100_FC_Attribute() 
{

}

S100_FC_Attribute::S100_FC_Attribute(rapidxml::xml_node<>* node):S100_FC_Item(node)
{

}

S100_FC_Attribute::S100_FC_Attribute(Attribute* a):S100_FC_Item((Item*)a)
{

}