#include "stdafx.h"
#include "AttributeBinding.h"

AttributeBinding::AttributeBinding()
{

}

AttributeBinding::~AttributeBinding()
{

}

void AttributeBinding::GetContents(pugi::xml_node& node)
{
	if (node.attribute("sequential")) //Add attribute value
	{
		XML_Attribute value;
		value.Setname("sequential");
		value.Setvalue((char*)node.attribute("sequential").value());
		SetAttributes(value);
	}

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "S100FC:multiplicity"))
		{
			multiplicity.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FC:permittedValues"))
		{
			permittedValues.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FC:attribute"))
		{
			attribute.GetContents(instruction);
		}
	}
}

void AttributeBinding::SetMultiplicity(Multiplicity value) 
{
	multiplicity = value;
}

Multiplicity& AttributeBinding::GetMultiplicity() 
{
	return multiplicity;
}

void AttributeBinding::SetPermittedValues(ValueList value) 
{
	permittedValues = value;
}

ValueList& AttributeBinding::GetPermittedValues()
{
	return permittedValues;
}

void AttributeBinding::SetAttribute(Reference value)
{
	attribute = value;
}

Reference& AttributeBinding::GetAttribute() 
{
	return attribute;
}