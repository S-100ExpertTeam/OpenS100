#include "stdafx.h"
#include "StringUtil.h"
#include "DefinitionReference.h"

DefinitionReference::DefinitionReference()
{

}

DefinitionReference::~DefinitionReference()
{
	delete sourceIdentifier;
	sourceIdentifier = nullptr;
}

void DefinitionReference::GetContents(pugi::xml_node& node) 
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const char* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:sourceIdentifier"))
		{
			if (nullptr == sourceIdentifier)
			{
				sourceIdentifier = new std::string();
			}

			*sourceIdentifier = instruction.child_value();
		}
		else if (!strcmp(instructionName, "S100FC:definitionSource"))
		{
			definitionSource.GetContents(instruction);
		}
	}
}

void DefinitionReference::SetSourceIdentifier(std::string value)
{
	if (nullptr == sourceIdentifier)
	{
		sourceIdentifier = new std::string();
	}
	
	*sourceIdentifier = value;
}

const std::string* DefinitionReference::GetSourceIdentifier()
{
	if (sourceIdentifier) 
	{
		return sourceIdentifier;
	}
	
	return nullptr;
}

void DefinitionReference::SetSourceIdentifier(const char* value)
{
	std::string s(value ? value : "");
	SetSourceIdentifier(s);
}

void DefinitionReference::SetSourceIdentifier(std::wstring value)
{
	std::string s = toUtf8(value);
	SetSourceIdentifier(s);
}

std::wstring DefinitionReference::GetSourceIdentifierW()
{
	return toWide(*GetSourceIdentifier());
}
