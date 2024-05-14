#include "stdafx.h"
#include "AttributeBinding.h"

#include "../LibMFCUtil/LibMFCUtil.h"

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
		sequential = node.attribute("sequential").as_bool();

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
			attributeCode = instruction.attribute("ref").as_string();
		}
		else if (!strcmp(instructionName, "S100FC:attributeVisibility"))
		{
			attributeVisibility = stringToAttributeVisibility(instruction.child_value());
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

std::string AttributeBinding::GetAttributeCode()
{
	return attributeCode;
}

std::wstring AttributeBinding::GetAttributeCodeAsWstring()
{
	return pugi::as_wide(attributeCode);
}

bool AttributeBinding::IsSequential()
{
	return sequential;
}

bool AttributeBinding::IsNullable()
{
	if (multiplicity.GetLower() == 0)
	{
		return true;
	}

	return false;
}

bool AttributeBinding::IsInfinite()
{
	return multiplicity.IsInfinite();
}

int AttributeBinding::GetLower()
{
	return multiplicity.GetLower();
}

int AttributeBinding::GetUpper()
{
	return multiplicity.GetUpperCount();
}