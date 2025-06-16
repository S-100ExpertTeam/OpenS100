#include "stdafx.h"
#include "S100_Description.h"


S100_Description::S100_Description()
{

}

S100_Description::~S100_Description()
{

}

void S100_Description::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		auto instructionName = instruction.name();

		if (!strcmp(instructionName, "name"))
		{
			std::wstring name_value = pugi::as_wide(instruction.child_value());
			name = name_value;
			//Setname(name_value);
		}

		if (!strcmp(instructionName, "description"))
		{
			auto description_value = pugi::as_wide(instruction.child_value());
			description = description_value;
			//Setdescription(description_value);
		}

		if (!strcmp(instructionName, "language"))
		{
			auto language_value = pugi::as_wide(instruction.child_value());
			language = language_value;
			//Setlanguage(language_value);
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