#pragma once
#include "XML_Item.h"
#include "DefinitionSource.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <string>

class DefinitionSources : public XML_Item
{
public:
	DefinitionSources();
	virtual ~DefinitionSources();

private:
	std::wstring sourceIdentifier;
	std::unordered_map<std::wstring, DefinitionSource> definitionSource;

public:
	void GetContents(pugi::xml_node& node);
	void SetSourceIdentifier(std::wstring value);
	std::wstring& GetSourceIdentifier();
	void SetDefinitionSource(std::unordered_map<std::wstring, DefinitionSource> value);
	std::unordered_map<std::wstring, DefinitionSource>& GetDefinitionSource();
};