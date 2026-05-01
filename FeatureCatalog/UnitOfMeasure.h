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
	void SetName(std::wstring value) { SetName(toUtf8(value)); }
	const std::string GetName();
	std::wstring GetNameW() { return toWide(GetName()); }
	const std::string& GetNameRef();
	std::wstring GetNameRefW() { return toWide(GetNameRef()); }

	void NullCheckDefinition();
	void SetDefinition(std::string& value);
	void SetDefinition(std::wstring value) { SetDefinition(toUtf8(value)); }
	const std::string* GetDefinition();
	std::wstring GetDefinitionW() { return toWide(GetDefinition()); }

	void NullCheckSymbol();
	void SetSymbol(std::string& value);
	void SetSymbol(std::wstring value) { SetSymbol(toUtf8(value)); }
	const std::string* GetSymbol();
	std::wstring GetSymbolW() { return toWide(GetSymbol()); }
};
