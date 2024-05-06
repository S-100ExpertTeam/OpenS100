#pragma once
#include "XML_Item.h"
#include "DefinitionSource.h"

#include <pugixml.hpp>

class DefinitionReference
{
public:
	DefinitionReference();
	virtual ~DefinitionReference();

private:
	std::wstring* sourceIdentifier = nullptr;
	DefinitionSource definitionSource;
	
public:
	void GetContents(pugi::xml_node& node);
	void SetSourceIdentifier(std::wstring value);
	const std::wstring* GetSourceIdentifier();
};