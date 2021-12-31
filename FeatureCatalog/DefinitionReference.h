#pragma once
#include "XML_Item.h"
#include "Reference.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class DefinitionReference :
	public XML_Item
{
public:
	DefinitionReference();
	virtual ~DefinitionReference();

private:
	std::wstring sourceIdentifier = L"";
	Reference definitionSource;
	
public:
	void GetContents(pugi::xml_node& node);
	void SetSourceIdentifier(std::wstring value);
	const std::wstring& GetSourceIdentifier();
	void SetDefinitionSource(Reference value);
	Reference& GetDefinitionSource();
};