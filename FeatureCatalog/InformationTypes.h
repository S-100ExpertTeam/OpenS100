#pragma once
#include "XML_Item.h"
#include "InformationType.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class InformationTypes : public XML_Item
{
public:
	InformationTypes(void);
	virtual ~InformationTypes(void);

private:
	std::unordered_map<std::wstring, InformationType> informationType;

public:
	void GetContents(pugi::xml_node& node);
	
	std::unordered_map<std::wstring, InformationType>& GetInformationTypePointer();
	bool SetAssociationFromSuperType(InformationType* it);
};