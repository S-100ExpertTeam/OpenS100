#pragma once
#include "ExternalFile.h"

#include <string>

#include "S100_Description.h"
#include "Enum_RuleType.h"

#include <pugixml.hpp>

class S100_RuleFile : public Portrayal::ExternalFile
{
public:
	S100_RuleFile();
	virtual ~S100_RuleFile();
	
private:
	Portrayal::RuleType ruleType = Portrayal::RuleType::none;

public:
	void GetContents(pugi::xml_node& node);

	Portrayal::RuleType GetRuleType();

	bool IsTopLevelTemplate();
	
	bool IsLua();

	const std::wstring GetRuleTypeAsWstring();
};