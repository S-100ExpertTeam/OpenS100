#pragma once
#include "XML_Item.h"
#include "Multiplicity.h"
#include "ValueList.h"
#include "Reference.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class InformationBinding :
	public XML_Item
{
public:
	InformationBinding();
	virtual ~InformationBinding();

private:
	Multiplicity multiplicity;
	Reference association;
	Reference role;
	Reference informationType;

public:
	void GetContents(pugi::xml_node& node);
	Multiplicity&  GetMultiplicityPointer();
	Reference& GetAssociationPointer();
	Reference& GetRolePointer();
	Reference& GetInformationTypePointer();
};