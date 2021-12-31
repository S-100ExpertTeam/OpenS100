#include "stdafx.h"
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
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:label"))
		{
			label = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100FC:definition"))
		{
			definition = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100FC:code"))
		{
			code.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FC:remarks"))
		{
			remarks = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100FC:alias"))
		{
			alias.push_back(pugi::as_wide(instruction.child_value()));
		}
		else if (!strcmp(instructionName, "S100FC:definitionReference"))
		{
			definitionReference.GetContents(instruction);
		}
	}
}

const std::wstring& ListedValue::GetLabel()
{
	return label;
}

void ListedValue::SetLabel(std::wstring& value)
{
	label = value;
}

const std::wstring& ListedValue::GetDefinition()
{
	return definition;
}

void ListedValue::SetDefinition(std::wstring& value)
{
	definition = value;
}

IntegerCode& ListedValue::GetCode()
{
	return code;
}

const std::wstring& ListedValue::GetRemarks()
{
	return remarks;
}

void ListedValue::SetRemarks(std::wstring& value)
{
	remarks = value;
}

std::list<std::wstring>& ListedValue::GetAliasPointer()
{
	return alias;
}

DefinitionReference& ListedValue::GetDefinitionReferencePointer()
{
	return definitionReference;
}