#include "stdafx.h"
#include "ComplexAttributes.h"

ComplexAttributes::ComplexAttributes(void)
{

}

ComplexAttributes::~ComplexAttributes(void)
{
	for (auto i = complexAttribute.begin(); i != complexAttribute.end(); i++)
	{
		delete i->second;
	}
}

void ComplexAttributes::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:S100_FC_ComplexAttribute"))
		{
			auto complex = new ComplexAttribute();
			complex->GetContents(instruction);
			complexAttribute[complex->GetCodeAsWString()] = complex;
		}
	}
}

void ComplexAttributes::SetComplexAttribute(std::wstring key, ComplexAttribute* value)
{
	complexAttribute.insert({ key,value });
}

void ComplexAttributes::SetComplexAttribute(std::unordered_map<std::wstring, ComplexAttribute*> value) 
{
	complexAttribute = value;
}

ComplexAttribute* ComplexAttributes::GetComplexAttribute(std::wstring key) 
{
	return complexAttribute[key];
}

std::unordered_map<std::wstring, ComplexAttribute*>& ComplexAttributes::GetComplexAttributePointer() 
{
	return complexAttribute;
}