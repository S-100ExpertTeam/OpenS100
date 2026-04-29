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
	
	const std::string& GetName();
	std::wstring GetNameW() { return toWide(GetName()); }
	void SetName(std::string& value);
	void SetName(std::wstring value) { SetName(toUtf8(value)); }

	const std::string& GetDefinition();
	std::wstring GetDefinitionW() { return toWide(GetDefinition()); }
	void SetDefinition(std::string& value);
	void SetDefinition(std::wstring value) { SetDefinition(toUtf8(value)); }

	const std::string GetCode();
	std::wstring GetCodeW() { return toWide(GetCode()); }
	std::wstring GetCodeAsWString();
	void SetCode(std::string& value);
	void SetCode(std::wstring value) { SetCode(toUtf8(value)); }

	const bool IsEmptyRemarks();
	void NullCheckRemarks();
	void SetRemarks(std::string& value);
	void SetRemarks(std::wstring value) { SetRemarks(toUtf8(value)); }
	const std::string GetRemarks();
	std::wstring GetRemarksW() { return toWide(GetRemarks()); }
	std::wstring GetRemarksAsWString();

	Item& operator = (const Item& item);

	void setSourceIdentifier(std::string value);
	std::wstring getSourceIdentifierAsWString();
	std::string getSourceIdentifier();
	std::wstring getSourceIdentifierW() { return toWide(getSourceIdentifier()); }
};
