#include "stdafx.h"
#include "FeatureUseType.h"

FeatureUseType::FeatureUseType()
{
	InsertValue(L"geographic");
	InsertValue(L"meta");
	InsertValue(L"cartographic");
	InsertValue(L"aggregation");
	InsertValue(L"theme");
}

FeatureUseType::~FeatureUseType()
{

}

void FeatureUseType::GetContents(pugi::xml_node& node)
{
	SetValueString(pugi::as_wide(node.child_value()));
}