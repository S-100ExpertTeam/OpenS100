#pragma once

#include "S100ObjectType.h"
#include "FeatureUseType.h"
#include "FeatureBinding.h"
#include "InformationBinding.h"
#include "SpatialPrimitiveType.h"

#include <pugixml.hpp>

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
	std::list<FeatureBinding*> vecFeatureBinding;
	std::list<SpatialPrimitiveType> permittedPrimitives;

public:
	void GetContents(pugi::xml_node& node);

	const std::wstring& GetSuperType();
	void SetSuperType(std::wstring value);

	//FeatureBinding* GetFeatureBinding(std::string featureTypeCode);
	//FeatureBinding* GetFeatureBinding(std::wstring featureTypeCode);

	std::list<std::wstring>& GetSubTypePointer(); 
	FeatureUseType& GetFeatureUseTypePointer();
	std::list<FeatureBinding*>& GetFeatureBindingPointer();
	std::list<SpatialPrimitiveType>& GetPermittedPrimitivesPointer();
	void InsertFeatureBinding(FeatureBinding* value);

	FeatureBinding* CanMerged(FeatureBinding& fb);
};