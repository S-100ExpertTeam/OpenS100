#include "stdafx.h"
#include "DefinitionReference.h"

DefinitionReference::DefinitionReference()
{

}

DefinitionReference::~DefinitionReference()
{

}

void DefinitionReference::GetContents(pugi::xml_node& node) 
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:sourceIdentifier"))
		{
			sourceIdentifier = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100FC:definitionSource"))
		{
			definitionSource.GetContents(instruction);
		}
	}
}

void DefinitionReference::SetSourceIdentifier(std::wstring value)
{
	sourceIdentifier = value;
}

const std::wstring& DefinitionReference::GetSourceIdentifier()
{
	return sourceIdentifier;
}

void DefinitionReference::SetDefinitionSource(Reference value) 
{
	definitionSource = value;
}

Reference& DefinitionReference::GetDefinitionSource() 
{
	return definitionSource;
}