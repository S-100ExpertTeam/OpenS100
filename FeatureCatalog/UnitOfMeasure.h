#pragma once
#include "XML_Item.h"

#include <pugixml.hpp>

#include <string>

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
	const std::string GetName();
	const std::string& GetNameRef();

	void NullCheckDefinition();
	void SetDefinition(std::string& value);
	const std::string* GetDefinition();

	void NullCheckSymbol();
	void SetSymbol(std::string& value);
	const std::string* GetSymbol();
};