#pragma once
#include "XML_Item.h"
#include "IntegerCode.h"
#include "DefinitionReference.h"

#include <pugixml.hpp>

class ListedValue :
	public XML_Item
{
public:
	ListedValue();
	virtual ~ListedValue();

private:
	std::string label = "";
	std::string definition = "";
	int code;
	std::string remarks = "";
	std::list<std::string> alias; 
	DefinitionReference definitionReference; 

public:
	void GetContents(pugi::xml_node& node);

	const std::string& GetLabel();
	void SetLabel(std::string& value);

	const std::string& GetDefinition();
	void SetDefinition(std::string& value);

	const std::string& GetRemarks(); 
	void SetRemarks(std::string& value);

	int GetCode();

	std::list<std::string>& GetAliasPointer(); 

	DefinitionReference& GetDefinitionReferencePointer(); 
};