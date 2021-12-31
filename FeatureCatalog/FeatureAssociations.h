#pragma once
#include "FeatureAssociation.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class FeatureAssociations :
	public XML_Item
{
public:
	FeatureAssociations(void);
	virtual ~FeatureAssociations(void);

private:
	std::unordered_map<std::wstring, FeatureAssociation> featureAssociation;
	
public:
	void GetContents(pugi::xml_node& node);
	std::unordered_map<std::wstring, FeatureAssociation>& GetFeatureAssociationPointer();
};