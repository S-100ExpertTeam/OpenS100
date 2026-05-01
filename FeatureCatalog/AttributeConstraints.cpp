#include "stdafx.h"
#include "StringUtil.h"
#include "AttributeConstraints.h"

AttributeConstraints::AttributeConstraints()
{

}

AttributeConstraints::~AttributeConstraints()
{

}

void AttributeConstraints::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const char* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FD:stringLength"))
		{
			stringLength = std::stoi(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100FD:textPattern"))
		{
			textPattern = instruction.child_value();
		}
		else if (!strcmp(instructionName, "S100FD:range"))
		{
			range.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FD:precision"))
		{
			precision = std::stoi(instruction.child_value());
		}
	}
}

int AttributeConstraints::GetStringLength() 
{
	return stringLength;
}

void AttributeConstraints::SetStringLength(int value) 
{
	stringLength = value;
}

const std::string& AttributeConstraints::GetTextPattern()
{
	return textPattern;
}

void AttributeConstraints::GetTextPattern(std::string value)
{
	textPattern = value;
}

NumericRange& AttributeConstraints::GetRangePointer()
{
	return range;
}

int AttributeConstraints::GetPrecision()
{
	return precision;
}

void AttributeConstraints::SetPrecision(int value)
{
	precision = value;
}

std::wstring AttributeConstraints::GetTextPatternW()
{
	return toWide(GetTextPattern());
}

void AttributeConstraints::GetTextPattern(std::wstring value)
{
	std::string s = toUtf8(value);
	return GetTextPattern(s);
}
