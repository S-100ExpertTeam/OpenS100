#pragma once
#include "XML_Item.h"
#include "StringCode.h"
#include "DefinitionReference.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <string>

// S100_FC_Item
class Item :
	public XML_Item
{
public:
	Item();
	Item(const Item& item);
	virtual ~Item();

private:
	std::wstring name = L"";
	std::wstring definition = L"";
	std::wstring code = L"";
	std::wstring *remarks = nullptr;
	std::list<std::wstring> alias;
	DefinitionReference definitionReference; 

public:
	void GetContents(pugi::xml_node& node);

	bool CompareCode(std::string& value);
	bool CompareCode(std::wstring& value);

	std::list<std::wstring>& GetAliasPointer();
	
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
};