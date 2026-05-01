#include "stdafx.h"
#include "StringUtil.h"
#include "UnitOfMeasure.h"

UnitOfMeasure::UnitOfMeasure()
{

}

UnitOfMeasure::~UnitOfMeasure()
{
	if (definition!=nullptr)
	{
		delete definition;
		definition = nullptr;
	}

	if (symbol!=nullptr)
	{
		delete symbol;
		symbol = nullptr;
	}

}

void UnitOfMeasure::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const char* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100Base:name"))
		{
			name = instruction.child_value();
		}
		else if (!strcmp(instructionName, "S100Base:definition"))
		{
			auto value = instruction.child_value();
			SetDefinition(value);
		}
		else if (!strcmp(instructionName, "S100Base:symbol"))
		{
			auto value = instruction.child_value();
			SetSymbol(value);
		}
	}
}

void UnitOfMeasure::SetName(std::string value)
{
	name = value;
}

const std::string UnitOfMeasure::GetName()
{
	return name;
}

const std::string& UnitOfMeasure::GetNameRef()
{
	return name;
}

void UnitOfMeasure::NullCheckDefinition()
{
	if (nullptr == definition)
	{
		definition = new std::string();
	}
}

void UnitOfMeasure::SetDefinition(std::string& value)
{
	NullCheckDefinition();
	std::string tmp = value;
	*definition = tmp;
}

const std::string* UnitOfMeasure::GetDefinition()
{
	if (definition) 
	{
		return definition;
	}

	return nullptr;
}

void UnitOfMeasure::NullCheckSymbol()
{
	if (nullptr == symbol)
	{
		symbol = new std::string();
	}
}

void UnitOfMeasure::SetSymbol(std::string& value) 
{
	NullCheckSymbol();
	*symbol = value;
}

const std::string* UnitOfMeasure::GetSymbol()
{
	if (symbol) 
	{
		return symbol;
	}

	return nullptr;
}

void UnitOfMeasure::SetName(const char* value)
{
	std::string s(value ? value : "");
	SetName(s);
}

void UnitOfMeasure::SetName(std::wstring value)
{
	std::string s = toUtf8(value);
	SetName(s);
}

std::wstring UnitOfMeasure::GetNameW()
{
	return toWide(GetName());
}

std::wstring UnitOfMeasure::GetNameRefW()
{
	return toWide(GetNameRef());
}

void UnitOfMeasure::SetDefinition(const char* value)
{
	std::string s(value ? value : "");
	SetDefinition(s);
}

void UnitOfMeasure::SetDefinition(std::wstring value)
{
	std::string s = toUtf8(value);
	SetDefinition(s);
}

std::wstring UnitOfMeasure::GetDefinitionW()
{
	return toWide(GetDefinition());
}

void UnitOfMeasure::SetSymbol(const char* value)
{
	std::string s(value ? value : "");
	SetSymbol(s);
}

void UnitOfMeasure::SetSymbol(std::wstring value)
{
	std::string s = toUtf8(value);
	SetSymbol(s);
}

std::wstring UnitOfMeasure::GetSymbolW()
{
	return toWide(GetSymbol());
}
