#pragma once
#include "XML_Item.h"
#include "Multiplicity.h"
#include "Reference.h"

class FeatureBinding :
	public XML_Item
{
public:
	FeatureBinding();
	virtual ~FeatureBinding();

private:
	Multiplicity multiplicity;
	Reference association;
	Reference role;
	Reference featureType;

public:
	void GetContents(pugi::xml_node& node);
	Multiplicity& GetMultiplicityPointer();
	Reference& GetAssociationPointer();
	Reference& GetRolePointer();
	Reference& GetFeatureTypePointer();
};