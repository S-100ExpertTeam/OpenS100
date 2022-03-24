#include "stdafx.h"
#include "S100_RuleFile.h"

#include <unordered_map>

S100_RuleFile::S100_RuleFile()
{

}

S100_RuleFile::~S100_RuleFile()
{

}

void S100_RuleFile::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}

	id = node.attribute("id").value();

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "fileName"))
		{
			fileName = pugi::as_wide(instruction.child_value());
		}

		else if (!strcmp(instructionName, "description"))
		{
			description.GetContents(instruction);
		}

		else if (!strcmp(instructionName, "fileType"))
		{
			fileType = pugi::as_wide(instruction.child_value());
		}

		else if (!strcmp(instructionName, "fileFormat"))
		{
			fileFormat = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "ruleType"))
		{
			ruleType = pugi::as_wide(instruction.child_value());
		}
	}
}


void S100_RuleFile::SetID(std::string& value)
{
	id = value;
}


std::string S100_RuleFile::GetID()
{
	return id;
}


void S100_RuleFile::SetFileName(std::wstring& value)
{
	fileName = value;
}


std::wstring S100_RuleFile::GetFileName()
{
	return fileName;
}


void S100_RuleFile::SetFileType(std::wstring& value)
{
	fileType = value;
}


std::wstring S100_RuleFile::GetFileType()
{
	return fileType;
}


void S100_RuleFile::SetFileFormat(std::wstring& value)
{
	fileFormat = value;
}


std::wstring S100_RuleFile::GetFileFormat()
{
	return fileFormat;
}


void S100_RuleFile::SetRuleType(std::wstring& value)
{
	ruleType = value;
}


std::wstring S100_RuleFile::GetRuleType()
{
	return ruleType;
}


bool S100_RuleFile::IsTypeLevelTemplate()
{
	if (GetRuleType().compare(L"TopLevelTemplate") == 0)
	{
		return true;
	}

	return false;
}


bool S100_RuleFile::IsLua()
{
	if (GetFileFormat().compare(L"LUA") == 0)
	{
		return true;
	}

	return false;
}