#pragma once
#include "XML_Item.h"
#include "StringCode.h"
#include "DefinitionReference.h"

#include <pugixml.hpp>

#include <string>

// S100_FC_Item
class Item
{
public:
	Item();
	Item(const Item& item);
	virtual ~Item();

protected:
	std::string name = "";
	std::string definition = "";
	std::string code = "";
	std::string* remarks = nullptr;
	std::list<std::string> alias;
	DefinitionReference* definitionReference = nullptr; 

public:
	void GetContents(pugi::xml_node& node);

	bool CompareCode(std::string& value);
	bool CompareCode(std::string& value);

	const std::list<std::string>& GetAlias();
	
	const std::string& GetName();
	void SetName(std::string& value);

	const std::string& GetDefinition();
	void SetDefinition(std::string& value);

	const std::string GetCode();
	const std::string GetCodeAsWString();
	void SetCode(std::string& value);

	const bool IsEmptyRemarks();
	void NullCheckRemarks();
	void SetRemarks(std::string& value);
	const std::string GetRemarks();
	const std::string GetRemarksAsWString();

	Item& operator = (const Item& item);

	void setSourceIdentifier(std::string value);
	std::string getSourceIdentifierAsWString();
	std::string getSourceIdentifier();
};