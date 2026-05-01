#pragma once
#include "XML_Item.h"
#include "StringCode.h"
#include "DefinitionReference.h"

#include <pugixml.hpp>

#include <string>
#include "StringUtil.h"

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

	const std::list<std::string>& GetAlias();
	int GetAliasCount();
	std::string GetAlias(int index);
	std::wstring GetAliasW(int index);
	
	const std::string& GetName();
	std::wstring GetNameW();
	void SetName(std::string& value);
	void SetName(const char* value);
	void SetName(std::wstring value);

	const std::string& GetDefinition();
	std::wstring GetDefinitionW();
	void SetDefinition(std::string& value);
	void SetDefinition(const char* value);
	void SetDefinition(std::wstring value);

	const std::string GetCode();
	std::wstring GetCodeW();
	std::wstring GetCodeAsWString();
	void SetCode(std::string& value);
	void SetCode(const char* value);
	void SetCode(std::wstring value);

	const bool IsEmptyRemarks();
	void NullCheckRemarks();
	void SetRemarks(std::string& value);
	void SetRemarks(const char* value);
	void SetRemarks(std::wstring value);
	const std::string GetRemarks();
	std::wstring GetRemarksW();
	std::wstring GetRemarksAsWString();

	Item& operator = (const Item& item);

	void setSourceIdentifier(std::string value);
	std::wstring getSourceIdentifierAsWString();
	std::string getSourceIdentifier();
	std::wstring getSourceIdentifierW();
};
