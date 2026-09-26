#include "stdafx.h"
#include "StringUtil.h"
#include "DateExt.h"

DateExt::DateExt()
{

}

DateExt::~DateExt()
{

}

void DateExt::GetContents(pugi::xml_node& node)
{
	//S100CI
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const char* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100CI:date")) 
		{
			SetDate((char*)instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:yearMonth"))
		{
			SetDate((char*)instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:year"))
		{
			SetDate((char*)instruction.child_value());
		}
	}
}

void DateExt::SetDate(char* value) 
{
	date = value;
}
const std::string& DateExt::GetDate()
{
	return date;
}

std::wstring DateExt::GetDateW()
{
	return toWide(GetDate());
}
