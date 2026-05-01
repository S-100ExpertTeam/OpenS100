#include "stdafx.h"
#include "StringUtil.h"
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
		const char* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100CI:name"))
		{
			name = instruction.child_value();
		}
		else if (!strcmp(instructionName, "S100CI:issueIdentification"))
		{
			issueIdentification = instruction.child_value();
		}
		else if (!strcmp(instructionName, "S100CI:page"))
		{
			page = instruction.child_value();
		}
	}
}

void Series::SetName() 
{
	name = Getvalue();
}

const std::string& Series::GetName()
{
	return name;
}

void Series::SetIssueIdentification(std::string value) 
{
	issueIdentification = value;
}
const std::string& Series::GetIssueIdentification()
{
	return issueIdentification;
}

void Series::SetPage(std::string valeu) 
{
	page = Getvalue();
}

const std::string& Series::GetPage()
{
	return page;
}

std::wstring Series::GetNameW()
{
	return toWide(GetName());
}

void Series::SetIssueIdentification(const char* value)
{
	std::string s(value ? value : "");
	SetIssueIdentification(s);
}

void Series::SetIssueIdentification(std::wstring value)
{
	std::string s = toUtf8(value);
	SetIssueIdentification(s);
}

std::wstring Series::GetIssueIdentificationW()
{
	return toWide(GetIssueIdentification());
}

void Series::SetPage(const char* value)
{
	std::string s(value ? value : "");
	SetPage(s);
}

void Series::SetPage(std::wstring valeu)
{
	std::string s = toUtf8(valeu);
	SetPage(s);
}

std::wstring Series::GetPageW()
{
	return toWide(GetPage());
}
