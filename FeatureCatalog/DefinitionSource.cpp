#include "stdafx.h"
#include "DefinitionSource.h"

DefinitionSource::DefinitionSource()
{

}

DefinitionSource::~DefinitionSource()
{

}

void DefinitionSource::GetContents(pugi::xml_node& node)
{
	auto attribute = node.attribute("id");
	if (attribute) 
	{
		XML_Attribute value;
		value.Setname("id");
		value.Setvalue((char*)attribute.value());

		SetAttributes(value);
	}
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:source"))
		{
		   source.GetContents(instruction);
		}
	}
}

void DefinitionSource::SetSource(Citation value)
{
	source = value;
}

Citation& DefinitionSource::GetSource() 
{
	return source;
}