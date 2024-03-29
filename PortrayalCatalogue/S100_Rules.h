#pragma once
#include "S100_RuleFile.h"

#include <pugixml.hpp>

#include <unordered_map>
#include <string>

class S100_Rules
{
public:
	S100_Rules();
	virtual ~S100_Rules();

private:
	std::vector<S100_RuleFile*> ruleFileVector;
	std::unordered_map<std::wstring, S100_RuleFile*> ruleFiles;

public:
	void AddRuleFiles(std::wstring& key, S100_RuleFile* value);
	void SetRuleFiles(std::unordered_map<std::wstring, S100_RuleFile*> value);

	S100_RuleFile* GetRuleFiles(std::wstring& key);
	std::unordered_map<std::wstring, S100_RuleFile*> GetRuleFiles();

	std::vector<S100_RuleFile*>* GetRuleFilesVector();

	void GetContents(pugi::xml_node& node);
};