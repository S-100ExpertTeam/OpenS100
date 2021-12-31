#include "stdafx.h"
#include "DefinitionSources.h"

DefinitionSources::DefinitionSources()
{

}

DefinitionSources::~DefinitionSources()
{

}

void DefinitionSources::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:S100_FC_sourceIdentifier"))
		{
			sourceIdentifier = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100FC:S100_FC_definitionSource"))
		{
			DefinitionSource definition;
			definition.GetContents(instruction);
			definitionSource[definition.Getvalue()] = definition;
		}
		else
		{
			std::wstring content = pugi::as_wide(instruction.child_value());
			content.append(L"is another data");
		}
	}
}

void DefinitionSources::SetSourceIdentifier(std::wstring value)
{
	sourceIdentifier = value;
}

std::wstring& DefinitionSources::GetSourceIdentifier()
{
	return sourceIdentifier;
}

void DefinitionSources::SetDefinitionSource(std::unordered_map<std::wstring, DefinitionSource> value)
{
	definitionSource = value;
}

std::unordered_map<std::wstring, DefinitionSource>& DefinitionSources::GetDefinitionSource()
{
	return definitionSource;
}