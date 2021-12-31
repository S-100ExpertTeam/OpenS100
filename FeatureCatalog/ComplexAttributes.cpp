#include "StdAfx.h"
#include "ComplexAttributes.h"

ComplexAttributes::ComplexAttributes(void)
{

}

ComplexAttributes::~ComplexAttributes(void)
{

}

void ComplexAttributes::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:S100_FC_ComplexAttribute"))
		{
			ComplexAttribute complex;
			complex.GetContents(instruction);
			complexAttribute[complex.GetCodeAsWString()] = complex;
		}
	}
}

void ComplexAttributes::SetComplexAttribute(std::wstring key, ComplexAttribute value)
{
	complexAttribute.insert({ key,value });
}

void ComplexAttributes::SetComplexAttribute(std::unordered_map<std::wstring, ComplexAttribute> value) 
{
	complexAttribute = value;
}

ComplexAttribute ComplexAttributes::GetComplexAttribute(std::wstring key) 
{
	return complexAttribute[key];
}

std::unordered_map<std::wstring, ComplexAttribute>& ComplexAttributes::GetComplexAttributePointer() 
{
	return complexAttribute;
}