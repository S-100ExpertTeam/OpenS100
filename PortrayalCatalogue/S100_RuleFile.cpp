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
	Portrayal::ExternalFile::GetContents(node);
	ruleType = Portrayal::StringToRuleType(std::string(node.child("ruleType").child_value()));
}

Portrayal::RuleType S100_RuleFile::GetRuleType()
{
	return ruleType;
}

bool S100_RuleFile::IsTopLevelTemplate()
{
	if (ruleType == Portrayal::RuleType::TopLevelTemplate)
	{
		return true;
	}

	return false;
}

bool S100_RuleFile::IsLua()
{
	if (GetFileFormat() == Portrayal::FileFormat::LUA)
	{
		return true;
	}

	return false;
}

const std::wstring S100_RuleFile::GetRuleTypeAsWstring()
{
	return Portrayal::RuleTypeToWString(ruleType);
}