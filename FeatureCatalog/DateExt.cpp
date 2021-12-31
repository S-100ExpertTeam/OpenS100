#include "stdafx.h"
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
		const pugi::char_t* instructionName = instruction.name();
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
	date = pugi::as_wide(value);
}
const std::wstring& DateExt::GetDate()
{
	return date;
}