#pragma once
#include "XML_Item.h"
#include "FeatureType.h"

#include <pugixml.hpp>

#include <vector>
#include <map>

class FeatureTypes :
	public XML_Item
{
public:
	FeatureTypes(void);
	virtual ~FeatureTypes(void);
	
private:
	std::vector<FeatureType*> vecFeatureType;
	std::unordered_map<std::wstring, FeatureType*> featureType;
	
public:
	void GetContents(pugi::xml_node& node);
	void ApplySuperType();
	bool SetAttributeFromSuperType(FeatureType* ft);
	bool SetAssociationFromSuperType(FeatureType* ft);
	std::vector<FeatureType*>& GetVecFeatureType();
    std::unordered_map<std::wstring, FeatureType*>& GetFeatureType();

	bool InsertFeatureType(FeatureType* value);
};