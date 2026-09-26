#pragma once
#include "XML_Item.h"
#include "DefinitionSource.h"

#include <pugixml.hpp>
#include "StringUtil.h"

class DefinitionReference
{
public:
	DefinitionReference();
	virtual ~DefinitionReference();

private:
	std::string* sourceIdentifier = nullptr;
	DefinitionSource definitionSource;
	
public:
	void GetContents(pugi::xml_node& node);
	void SetSourceIdentifier(std::string value);
	void SetSourceIdentifier(const char* value);
	void SetSourceIdentifier(std::wstring value);
	const std::string* GetSourceIdentifier();
	std::wstring GetSourceIdentifierW();
};
