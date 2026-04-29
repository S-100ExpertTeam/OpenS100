#pragma once
#include "XML_Item.h"
#include "DefinitionSource.h"

#include <pugixml.hpp>

#include <string>
#include "StringUtil.h"

class DefinitionSources : public XML_Item
{
public:
	DefinitionSources();
	virtual ~DefinitionSources();

private:
	std::string sourceIdentifier;
	std::unordered_map<std::string, DefinitionSource*> definitionSource;

public:
	void GetContents(pugi::xml_node& node);
	void SetSourceIdentifier(std::string value);
	void SetSourceIdentifier(std::wstring value) { SetSourceIdentifier(toUtf8(value)); }
	std::string& GetSourceIdentifier();
	std::wstring GetSourceIdentifierW() { return toWide(GetSourceIdentifier()); }
	void SetDefinitionSource(std::unordered_map<std::string, DefinitionSource*> value);
	std::unordered_map<std::string, DefinitionSource*>& GetDefinitionSource();
	bool InsertDefinitionSource(DefinitionSource* value);
};
