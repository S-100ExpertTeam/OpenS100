#include "stdafx.h"
#include "S100_Rules.h"

S100_Rules::S100_Rules()
{
}

S100_Rules::~S100_Rules()
{
	for (auto itor = ruleFiles.begin(); itor != ruleFiles.end(); itor++)
	{
		if(itor->second != nullptr)
			delete itor->second;
	}
}

void S100_Rules::AddRuleFiles(const std::string& key, S100_RuleFile* value)  { ruleFiles[key] = value; }
void S100_Rules::AddRuleFiles(const std::wstring& key, S100_RuleFile* value) { ruleFiles[toUtf8(key)] = value; }

void S100_Rules::SetRuleFiles(std::unordered_map<std::string, S100_RuleFile*> value)
{
	ruleFiles = value;
}

S100_RuleFile* S100_Rules::GetRuleFiles(const std::string& key)
{
	auto it = ruleFiles.find(key);
	return (it != ruleFiles.end()) ? it->second : nullptr;
}

S100_RuleFile* S100_Rules::GetRuleFiles(const std::wstring& key)
{
	return GetRuleFiles(toUtf8(key));
}

std::unordered_map<std::string, S100_RuleFile*> S100_Rules::GetRuleFiles()
{
	return ruleFiles;
}

void S100_Rules::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "ruleFile"))
		{
			S100_RuleFile* ruleFile = new S100_RuleFile();
			ruleFile->GetContents(instruction);
			ruleFiles[ruleFile->GetId()] = ruleFile;
			ruleFileVector.push_back(ruleFile);
		}
	}
}

std::vector<S100_RuleFile*>* S100_Rules::GetRuleFilesVector()
{
	return &ruleFileVector;
}
