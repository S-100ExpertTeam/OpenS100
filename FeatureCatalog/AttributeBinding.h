#pragma once
#include "XML_Item.h"
#include "ValueList.h"
#include "Multiplicity.h"
#include "Reference.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class AttributeBinding :
	public XML_Item
{
public:
	AttributeBinding();
	virtual ~AttributeBinding();
	
private:
	Multiplicity multiplicity;
	ValueList permittedValues;
	Reference attribute;

public:
	void GetContents(pugi::xml_node& node);
	void SetMultiplicity(Multiplicity value);
	Multiplicity& GetMultiplicity();
	void SetPermittedValues(ValueList value);
	ValueList& GetPermittedValues();
	void SetAttribute(Reference value);
	Reference& GetAttribute();
};