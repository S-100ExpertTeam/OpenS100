#pragma once
#include "XML_Item.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <string>

// S100_UnitOfMeasure
class UnitOfMeasure : public XML_Item
{
public:
	UnitOfMeasure();
	virtual ~UnitOfMeasure();

private:
	std::wstring name = L"";
	std::wstring* definition = nullptr;
	std::wstring* symbol = nullptr;

public:
	void GetContents(pugi::xml_node& node);

	void SetName(std::wstring value);
	const std::wstring GetName();
	const std::wstring& GetNameRef();

	void NullCheckDefinition();
	void SetDefinition(std::wstring& value);
	const std::wstring& GetDefinition();

	void NullCheckSymbol();
	void SetSymbol(std::wstring& value);
	const std::wstring& GetSymbol();
};