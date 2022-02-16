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

void S100_Dash::SetStart(std::wstring& value)
{
	start = value;
}

std::wstring S100_Dash::GetStart()
{
	return start;
}

void S100_Dash::SetLength(std::wstring& value)
{
	length = value;
}

std::wstring S100_Dash::GetLength()
{
	return length;
}

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
			start = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "length"))
		{
			length = pugi::as_wide(instruction.child_value());
		}
	}
}

void S100_Dash::SetFromStartLength(std::string& value)
{
	auto commaIndex = value.find(',');

	// If you don't have a comma,
	if (std::string::npos == commaIndex)
	{
		OutputDebugString(_T("Failed to parse (Dash:start,length)[1]\n"));
		return;
	}
	else if (commaIndex >= value.length())
	{
		OutputDebugString(_T("Failed to parse (Dash:start,length)[2]\n"));
		return;
	}
	else
	{
		auto start = value.substr(0, commaIndex);
		auto length = value.substr(commaIndex + 1, std::string::npos);

		SetStart(pugi::as_wide(start));
		SetLength(pugi::as_wide(length));
	}
}

bool S100_Dash::IsEmpty()
{
	if (start.empty() && length.empty())
	{
		return true;
	}

	return false;
}

void S100_Dash::SetEmpty()
{
	start = L"";
	length = L"";
}

void S100_Dash::ParseValue(std::string value)
{
	std::vector<std::string> v_splited = LatLonUtility::Split(value, ",");
	if (v_splited.size() >= 2)
	{
		start = std::wstring(v_splited[0].begin(), v_splited[0].end());
		length = std::wstring(v_splited[1].begin(), v_splited[1].end());
	}
}