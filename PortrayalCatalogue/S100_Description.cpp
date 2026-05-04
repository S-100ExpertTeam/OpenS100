#include "stdafx.h"
#include "S100_Description.h"

#include <pugixml.hpp>

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
			name = instruction.child_value();
		}

		if (!strcmp(instructionName, "description"))
		{
			description = instruction.child_value();
		}

		if (!strcmp(instructionName, "language"))
		{
			language = instruction.child_value();
		}
	}
}

void S100_Description::Setname(const std::string& value)  { name = value; }
void S100_Description::Setname(const std::wstring& value) { name = toUtf8(value); }
std::string  S100_Description::Getname()  { return name; }
std::wstring S100_Description::GetnameW() { return toWide(name); }

void S100_Description::Setdescription(const std::string& value)  { description = value; }
void S100_Description::Setdescription(const std::wstring& value) { description = toUtf8(value); }
std::string  S100_Description::Getdescription()  { return description; }
std::wstring S100_Description::GetdescriptionW() { return toWide(description); }

void S100_Description::Setlanguage(const std::string& value)  { language = value; }
void S100_Description::Setlanguage(const std::wstring& value) { language = toUtf8(value); }
std::string  S100_Description::Getlanguage()  { return language; }
std::wstring S100_Description::GetlanguageW() { return toWide(language); }

void S100_Description::SetOn(bool value)
{
	on = value;
}

bool S100_Description::GetOn()
{
	return on;
}
