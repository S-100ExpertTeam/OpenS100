#include "stdafx.h"
#include "Enum_RuleType.h"

#include <pugixml.hpp>

namespace Portrayal
{
	RuleType StringToRuleType(std::string& value)
	{
		int maxindex = (int)RuleType::SubTemplate + 1;
		for (int i = 0; i < maxindex; i++)
		{
			if (0 == value.compare(RuleType_String[i]))
			{
				return RuleType(i);
			}
		}
		return RuleType::none;
	}

	RuleType StringToRuleType(std::wstring& value)
	{
		auto str = pugi::as_utf8(value);
		return StringToRuleType(str);
	}

	std::string RuleTypeToString(RuleType value)
	{
		int index = (int)value;
		if (index < (int)RuleType::none ||
			index >(int)RuleType::SubTemplate)
		{
			return "";
		}
		return RuleType_String[index];
	}

	std::wstring RuleTypeToWString(RuleType value)
	{
		auto str = RuleTypeToString(value);
		return pugi::as_wide(str);
	}
}