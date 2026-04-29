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
	std::string superType; 
	std::list<std::string> subType; 
	FeatureUseType featureUseType;
	std::list<FeatureBinding*> vecFeatureBinding;
	std::list<SpatialPrimitiveType> permittedPrimitives;

public:
	void GetContents(pugi::xml_node& node);

	const std::string& GetSuperType();
	void SetSuperType(std::string value);

	//FeatureBinding* GetFeatureBinding(std::string featureTypeCode);
	//FeatureBinding* GetFeatureBinding(std::string featureTypeCode);

	std::list<std::string>& GetSubTypePointer(); 
	FeatureUseType& GetFeatureUseTypePointer();
	std::list<FeatureBinding*>& GetFeatureBindingPointer();
	std::list<SpatialPrimitiveType>& GetPermittedPrimitivesPointer();
	void InsertFeatureBinding(FeatureBinding* value);

	FeatureBinding* CanMerged(FeatureBinding& fb);
};