#pragma once
#include "XML_Item.h"
#include "InformationType.h"

#include <pugixml.hpp>

#include <vector>
#include <map>

class InformationTypes
{
public:
	InformationTypes(void);
	virtual ~InformationTypes(void);

private:
	std::vector<InformationType*> informationTypes;

	// key : code (information type)
	std::unordered_map<std::wstring, InformationType*> informationType;

public:
	void GetContents(pugi::xml_node& node);
	
	std::vector<InformationType*> GetVecInformationType();
	std::unordered_map<std::wstring, InformationType*>& GetInformationTypePointer();
	void ApplySuperType();
	bool SetAttributeFromSuperType(InformationType* it);
	bool SetAssociationFromSuperType(InformationType* it);

	bool InsertInformationType(InformationType* value);
};