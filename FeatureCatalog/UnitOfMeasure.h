#pragma once
#include "XML_Item.h"

#include <pugixml.hpp>

#include <string>
#include "StringUtil.h"

// S100_UnitOfMeasure
class UnitOfMeasure
{
public:
	UnitOfMeasure();
	virtual ~UnitOfMeasure();

private:
	std::string name = "";
	std::string* definition = nullptr;
	std::string* symbol = nullptr;

public:
	void GetContents(pugi::xml_node& node);

	void SetName(std::string value);
	void SetName(const char* value);
	void SetName(std::wstring value);
	const std::string GetName();
	std::wstring GetNameW();
	const std::string& GetNameRef();
	std::wstring GetNameRefW();

	void NullCheckDefinition();
	void SetDefinition(std::string& value);
	void SetDefinition(const char* value);
	void SetDefinition(std::wstring value);
	const std::string* GetDefinition();
	std::wstring GetDefinitionW();

	void NullCheckSymbol();
	void SetSymbol(std::string& value);
	void SetSymbol(const char* value);
	void SetSymbol(std::wstring value);
	const std::string* GetSymbol();
	std::wstring GetSymbolW();
};
