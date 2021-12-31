#include "stdafx.h"
#include "CI_Organisation.h"

CI_Organisation::CI_Organisation()
{

}

CI_Organisation::~CI_Organisation()
{

}

void CI_Organisation::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName,"S100CI:name"))
		{
			SetName((char*)instruction.child_value());
		}
		else if (!strcmp(instructionName,"S100CI:contactInfo"))
		{
			ContactInfo value;
			value.GetContents(instruction);
			SetContactinfo(value);
		}
	}
}

void CI_Organisation::SetName(char* value)
{
	name = value;
}

const std::string CI_Organisation::GetName()
{
	return name;
}

void CI_Organisation::SetContactinfo(ContactInfo& value) 
{
	contactinfo = value;
}

const ContactInfo& CI_Organisation::GetContactinfo()
{
	return contactinfo;
}