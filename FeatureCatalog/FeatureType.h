#pragma once

#include "S100ObjectType.h"
#include "FeatureUseType.h"
#include "FeatureBinding.h"
#include "InformationBinding.h"
#include "SpatialPrimitiveType.h"

#include <pugixml.hpp>
#include "StringUtil.h"

class FeatureType :
	public S100ObjectType
{
public:
	FeatureType();
	virtual ~FeatureType();

private:
	std::string superType; 
	std::list<std::string> subType; 
	FeatureUseType featureUseType;
	std::list<FeatureBinding*> vecFeatureBinding;
	std::list<SpatialPrimitiveType> permittedPrimitives;

public:
	void GetContents(pugi::xml_node& node);

	const std::string& GetSuperType();
	std::wstring GetSuperTypeW();
	void SetSuperType(std::string value);
	void SetSuperType(const char* value);
	void SetSuperType(std::wstring value);

	//FeatureBinding* GetFeatureBinding(std::string featureTypeCode);

	std::list<std::string>& GetSubTypePointer(); 
	FeatureUseType& GetFeatureUseTypePointer();
	std::list<FeatureBinding*>& GetFeatureBindingPointer();
	std::list<SpatialPrimitiveType>& GetPermittedPrimitivesPointer();
	void InsertFeatureBinding(FeatureBinding* value);

	FeatureBinding* CanMerged(FeatureBinding& fb);
};
