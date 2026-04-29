#pragma once
#include "InformationAssociation.h"

#include <pugixml.hpp>

class InformationAssociations :
	public XML_Item
{
public:
	InformationAssociations();
	virtual ~InformationAssociations();

private:
	std::unordered_map<std::string, InformationAssociation*> informationAssociation;

public:
	void GetContents(pugi::xml_node& node);
	std::unordered_map<std::string, InformationAssociation*>& GetInformationAssociationPointer();
};