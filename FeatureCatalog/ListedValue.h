#pragma once
#include "XML_Item.h"
#include "IntegerCode.h"
#include "DefinitionReference.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class ListedValue :
	public XML_Item
{
public:
	ListedValue();
	virtual ~ListedValue();

private:
	std::wstring label = L"";
	std::wstring definition = L"";
	IntegerCode code;
	std::wstring remarks = L"";
	std::list<std::wstring> alias; 
	DefinitionReference definitionReference; 

public:
	void GetContents(pugi::xml_node& node);

	const std::wstring& GetLabel();
	void SetLabel(std::wstring& value);

	const std::wstring& GetDefinition();
	void SetDefinition(std::wstring& value);

	const std::wstring& GetRemarks(); 
	void SetRemarks(std::wstring& value);

	IntegerCode& GetCode();

	std::list<std::wstring>& GetAliasPointer(); 

	DefinitionReference& GetDefinitionReferencePointer(); 
};