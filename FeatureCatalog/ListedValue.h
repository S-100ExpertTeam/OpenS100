#pragma once
#include "XML_Item.h"
#include "IntegerCode.h"
#include "DefinitionReference.h"

#include <pugixml.hpp>
#include "StringUtil.h"

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
	std::wstring GetLabelW() { return toWide(GetLabel()); }
	void SetLabel(std::string& value);
	void SetLabel(std::wstring value) { SetLabel(toUtf8(value)); }

	const std::string& GetDefinition();
	std::wstring GetDefinitionW() { return toWide(GetDefinition()); }
	void SetDefinition(std::string& value);
	void SetDefinition(std::wstring value) { SetDefinition(toUtf8(value)); }

	const std::string& GetRemarks(); 
	void SetRemarks(std::string& value);
	void SetRemarks(std::wstring value) { SetRemarks(toUtf8(value)); }

	int GetCode();

	std::list<std::string>& GetAliasPointer(); 

	DefinitionReference& GetDefinitionReferencePointer(); 
};
