#include "stdafx.h"
#include "FeatureUseType.h"

FeatureUseType::FeatureUseType()
{
	InsertValue("geographic");
	InsertValue("meta");
	InsertValue("cartographic");
	InsertValue("aggregation");
	InsertValue("theme");
}

FeatureUseType::~FeatureUseType()
{

}

void FeatureUseType::GetContents(pugi::xml_node& node)
{
	SetValueString(node.child_value());
}