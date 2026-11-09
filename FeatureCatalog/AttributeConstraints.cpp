#include "stdafx.h"
#include "StringUtil.h"
#include "AttributeConstraints.h"

#include "..\\LatLonUtility\\cpp_util.h"

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
			stringLength = cpp_util::stoi(instruction.child_value());
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
			precision = cpp_util::stoi(instruction.child_value());
		}
	}
}

int AttributeConstraints::GetStringLength() 
{
	return stringLength;
}

void AttributeConstraints::SetStringLength(int _value) 
{
	stringLength = _value;
}

const std::string& AttributeConstraints::GetTextPattern()
{
	return textPattern;
}

void AttributeConstraints::GetTextPattern(std::string _value)
{
	textPattern = _value;
}

NumericRange& AttributeConstraints::GetRangePointer()
{
	return range;
}

int AttributeConstraints::GetPrecision()
{
	return precision;
}

void AttributeConstraints::SetPrecision(int _value)
{
	precision = _value;
}

std::wstring AttributeConstraints::GetTextPatternW()
{
	return toWide(GetTextPattern());
}

void AttributeConstraints::GetTextPattern(std::wstring _value)
{
	std::string s = toUtf8(_value);
	return GetTextPattern(s);
}
