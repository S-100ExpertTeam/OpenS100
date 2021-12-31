#include "stdafx.h"
#include "SpatialPrimitiveType.h"

SpatialPrimitiveType::SpatialPrimitiveType()
{
	InsertValue(L"noGeometry");
	InsertValue(L"point");
	InsertValue(L"pointSet");
	InsertValue(L"curve");
	InsertValue(L"surface");
	InsertValue(L"coverage");
	InsertValue(L"arcByCenterPoint");
	InsertValue(L"circleByCenterPoint");
}

SpatialPrimitiveType::~SpatialPrimitiveType()
{

}

void SpatialPrimitiveType::GetContents(pugi::xml_node& node)
{
	SetValueString(pugi::as_wide(node.child_value()));
}