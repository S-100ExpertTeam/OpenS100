#include "stdafx.h"
#include "StringUtil.h"
#include "ListedValue.h"

ListedValue::ListedValue()
{

}

ListedValue::~ListedValue()
{

}

void ListedValue::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const char* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:label"))
		{
			label = instruction.child_value();
		}
		else if (!strcmp(instructionName, "S100FC:definition"))
		{
			definition = instruction.child_value();
		}
		else if (!strcmp(instructionName, "S100FC:code"))
		{
			code = instruction.text().as_int();
		}
		else if (!strcmp(instructionName, "S100FC:remarks"))
		{
			remarks = instruction.child_value();
		}
		else if (!strcmp(instructionName, "S100FC:alias"))
		{
			alias.push_back(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100FC:definitionReference"))
		{
			definitionReference.GetContents(instruction);
		}
	}
}

const std::string& ListedValue::GetLabel()
{
	return label;
}

void ListedValue::SetLabel(std::string& _value)
{
	label = _value;
}

const std::string& ListedValue::GetDefinition()
{
	return definition;
}

void ListedValue::SetDefinition(std::string& _value)
{
	definition = _value;
}

int ListedValue::GetCode()
{
	return code;
}

const std::string& ListedValue::GetRemarks()
{
	return remarks;
}

void ListedValue::SetRemarks(std::string& _value)
{
	remarks = _value;
}

std::list<std::string>& ListedValue::GetAliasPointer()
{
	return alias;
}

DefinitionReference& ListedValue::GetDefinitionReferencePointer()
{
	return definitionReference;
}

std::wstring ListedValue::GetLabelW()
{
	return toWide(GetLabel());
}

void ListedValue::SetLabel(const char* _value)
{
	std::string s(_value ? _value : "");
	SetLabel(s);
}

void ListedValue::SetLabel(std::wstring _value)
{
	std::string s = toUtf8(_value);
	SetLabel(s);
}

std::wstring ListedValue::GetDefinitionW()
{
	return toWide(GetDefinition());
}

void ListedValue::SetDefinition(const char* _value)
{
	std::string s(_value ? _value : "");
	SetDefinition(s);
}

void ListedValue::SetDefinition(std::wstring _value)
{
	std::string s = toUtf8(_value);
	SetDefinition(s);
}

void ListedValue::SetRemarks(const char* _value)
{
	std::string s(_value ? _value : "");
	SetRemarks(s);
}

void ListedValue::SetRemarks(std::wstring _value)
{
	std::string s = toUtf8(_value);
	SetRemarks(s);
}

bool ListedValue::CompareRemarks(const char* _value)
{
	if (_value == nullptr)
	{
		return false;
	}

	if (strcmp(remarks.c_str(), _value) == 0)
	{
		return true;
	}

	return false;
}