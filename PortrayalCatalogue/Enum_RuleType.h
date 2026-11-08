#pragma once

#include <string>

namespace Portrayal
{
	enum class RuleType
	{
		none = 0,
		TopLevelTemplate = 1,
		SubTemplate = 2
	};

	static const char* RuleType_String[] =
	{
		"none",
		"TopLevelTemplate",
		"SubTemplate"
	};

	RuleType StringToRuleType(const std::string& value);
	RuleType StringToRuleType(const std::wstring& value);

	std::string RuleTypeToString(RuleType value);
	std::wstring RuleTypeToWString(RuleType value);
}