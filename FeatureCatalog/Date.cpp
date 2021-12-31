#include "stdafx.h"
#include "Date.h"

Date::Date()
{

}

Date::~Date()
{

}

void Date::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100CI:date"))
		{
			DateExt* dateData = new DateExt();
			dateData->GetContents(instruction);
			SetDate(*dateData);
		}
		else if (!strcmp(instructionName, "S100CI:dateType"))
		{
			DateTypeCode* DatetypeCode = new DateTypeCode();
			DatetypeCode->GetContents(instruction);
			SetDateType(*DatetypeCode);
		}
		else  //unspecified value 
		{
		}
	}
}

void Date::SetDate(DateExt& value) 
{
	date = value;
}

DateExt& Date::GetDate() 
{
	return date;
}

void Date::SetDateType(DateTypeCode& value) 
{
	dateType = value;
}

DateTypeCode& Date::GetDateType() 
{
	return dateType;
}