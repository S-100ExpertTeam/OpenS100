#pragma once

#include "S100ObjectType.h"
#include "FeatureUseType.h"
#include "FeatureBinding.h"
#include "InformationBinding.h"
#include "SpatialPrimitiveType.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class FeatureType :
	public S100ObjectType
{
public:
	FeatureType();
	virtual ~FeatureType();

private:
	std::wstring superType; 
	std::list<std::wstring> subType; 
	FeatureUseType featureUseType;
	std::unordered_map<std::wstring, FeatureBinding> featureBinding;
	std::list<SpatialPrimitiveType> permittedPrimitives;

public:
	void GetContents(pugi::xml_node& node);

	const std::wstring& GetSuperType();
	void SetSuperType(std::wstring value);

	std::list<std::wstring>& GetSubTypePointer(); 
	FeatureUseType& GetFeatureUseTypePointer();
	std::unordered_map<std::wstring, FeatureBinding>& GetFeatureBindingPointer();
	std::list<SpatialPrimitiveType>& GetPermittedPrimitivesPointer();
};