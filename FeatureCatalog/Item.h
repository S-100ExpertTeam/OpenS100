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
	std::wstring name = L"";
	std::wstring definition = L"";
	std::wstring code = L"";
	std::wstring* remarks = nullptr;
	std::list<std::wstring> alias;
	DefinitionReference* definitionReference = nullptr; 

public:
	void GetContents(pugi::xml_node& node);

	bool CompareCode(std::string& value);
	bool CompareCode(std::wstring& value);

	const std::list<std::wstring>& GetAlias();
	
	const std::wstring& GetName();
	void SetName(std::wstring& value);

	const std::wstring& GetDefinition();
	void SetDefinition(std::wstring& value);

	const std::string GetCode();
	const std::wstring GetCodeAsWString();
	void SetCode(std::wstring& value);
	void SetCode(std::string& value);

	const bool IsEmptyRemarks();
	void NullCheckRemarks();
	void SetRemarks(std::string& value);
	void SetRemarks(std::wstring& value);
	const std::string GetRemarks();
	const std::wstring GetRemarksAsWString();

	Item& operator = (const Item& item);

	void setSourceIdentifier(std::wstring value);
	std::wstring getSourceIdentifierAsWString();
	std::string getSourceIdentifier();
};