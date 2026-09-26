#include "stdafx.h"
#include "S100_Dash.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

#include <string>
#include <vector>

S100_Dash::S100_Dash()
{

}

S100_Dash::~S100_Dash()
{

}

void S100_Dash::SetStart(const std::string& value)  { start = value; }
void S100_Dash::SetStart(const std::wstring& value) { start = toUtf8(value); }
std::string  S100_Dash::GetStart()  { return start; }
std::wstring S100_Dash::GetStartW() { return toWide(start); }

void S100_Dash::SetLength(const std::string& value)  { length = value; }
void S100_Dash::SetLength(const std::wstring& value) { length = toUtf8(value); }
std::string  S100_Dash::GetLength()  { return length; }
std::wstring S100_Dash::GetLengthW() { return toWide(length); }

void S100_Dash::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "start"))
		{
			start = instruction.child_value();
		}
		else if (!strcmp(instructionName, "length"))
		{
			length = instruction.child_value();
		}
	}
}

void S100_Dash::SetFromStartLength(std::string& value)
{
	auto commaIndex = value.find(',');

	if (std::string::npos == commaIndex || commaIndex >= value.length())
	{
		return;
	}

	start = value.substr(0, commaIndex);
	length = value.substr(commaIndex + 1, std::string::npos);
}

bool S100_Dash::IsEmpty()
{
	return start.empty() && length.empty();
}

void S100_Dash::SetEmpty()
{
	start = "";
	length = "";
}

void S100_Dash::ParseValue(std::string value)
{
	std::vector<std::string> v_splited = LatLonUtility::Split(value, ",");
	if (v_splited.size() >= 2)
	{
		start = v_splited[0];
		length = v_splited[1];
	}
}

void S100_Dash::ParseValue(std::string_view value)
{
	std::vector<std::string_view> v_splited;
	LatLonUtility::Split(value, ",", v_splited);
	if (v_splited.size() >= 2)
	{
		start = std::string(v_splited[0]);
		length = std::string(v_splited[1]);
	}
}
