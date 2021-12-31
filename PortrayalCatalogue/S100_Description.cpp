#include "stdafx.h"
#include "S100_Description.h"


S100_Description::S100_Description()
{

}

S100_Description::~S100_Description()
{

}

void S100_Description::GetContents(MSXML2::IXMLDOMNodeListPtr pNodeList)
{
	if (!pNodeList)
		return;

	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);

		if (!pNode)
			continue;
		std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();

		if (nodeName.compare(L"name") == 0)
		{
			name = std::wstring(pNode->Gettext());
		}
		else if (nodeName.compare(L"description") == 0)
		{
			description = std::wstring(pNode->Gettext());
		}
		else if (nodeName.compare(L"language") == 0)
		{
			language = std::wstring(pNode->Gettext());
		}
	}
}

void S100_Description::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "name"))
		{
			Setname(pugi::as_wide(instruction.child_value()));
		}

		if (!strcmp(instructionName, "description"))
		{
			Setdescription(pugi::as_wide(instruction.child_value()));
		}

		if (!strcmp(instructionName, "language"))
		{
			Setlanguage(pugi::as_wide(instruction.child_value()));
		}

	}
}

//==============Property=============================//

void S100_Description::Setname(std::wstring& value)
{
	name = value;
}

std::wstring S100_Description::Getname() 
{
	return name;
}

void S100_Description::Setdescription(std::wstring& value)
{
	description = value;
}

std::wstring S100_Description::Getdescription() 
{
	return description;
}

void S100_Description::Setlanguage(std::wstring& value)
{
	language = value;
}

std::wstring S100_Description::Getlanguage() 
{
	return language;
}

void S100_Description::SetOn(bool value) 
{
	on = value;
}

bool S100_Description::GetOn()
{
	return on;
}