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

void ListedValue::SetLabel(std::string& value)
{
	label = value;
}

const std::string& ListedValue::GetDefinition()
{
	return definition;
}

void ListedValue::SetDefinition(std::string& value)
{
	definition = value;
}

int ListedValue::GetCode()
{
	return code;
}

const std::string& ListedValue::GetRemarks()
{
	return remarks;
}

void ListedValue::SetRemarks(std::string& value)
{
	remarks = value;
}

std::list<std::string>& ListedValue::GetAliasPointer()
{
	return alias;
}

DefinitionReference& ListedValue::GetDefinitionReferencePointer()
{
	return definitionReference;
}