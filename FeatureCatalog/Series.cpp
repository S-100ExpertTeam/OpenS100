#include "stdafx.h"
#include "Series.h"

Series::Series()
{

}

Series::~Series()
{

}

void Series::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100CI:name"))
		{
			name = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:issueIdentification"))
		{
			issueIdentification = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:page"))
		{
			page = pugi::as_wide(instruction.child_value());
		}
	}
}

void Series::SetName() 
{
	name = Getvalue();
}

const std::wstring& Series::GetName()
{
	return name;
}

void Series::SetIssueIdentification(std::wstring value) 
{
	issueIdentification = value;
}
const std::wstring& Series::GetIssueIdentification()
{
	return issueIdentification;
}

void Series::SetPage(std::wstring valeu) 
{
	page = Getvalue();
}

const std::wstring& Series::GetPage()
{
	return page;
}