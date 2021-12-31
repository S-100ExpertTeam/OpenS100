#pragma once
#include "XML_Item.h"
#include "FeatureType.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class FeatureTypes :
	public XML_Item
{
public:
	FeatureTypes(void);
	virtual ~FeatureTypes(void);
	
private:
	std::unordered_map<std::wstring, FeatureType> featureType;
	
public:
	void GetContents(pugi::xml_node& node);
	void ApplySuperType();
	bool SetAttributeFromSuperType(FeatureType* ft);
	bool SetAssociationFromSuperType(FeatureType* ft);
	std::unordered_map<std::wstring, FeatureType>& GetFeatureTypePointer();
};