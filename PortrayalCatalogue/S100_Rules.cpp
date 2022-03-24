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

void S100_Rules::AddRuleFiles(std::string& key, S100_RuleFile* value)
{
	ruleFiles[key] = value;
}

void S100_Rules::SetRuleFiles(std::unordered_map<std::string, S100_RuleFile*> value)
{
	ruleFiles = value;
}

S100_RuleFile* S100_Rules::GetRuleFiles(std::string& key)
{
	auto isrule = ruleFiles.find(key);
	if (isrule!=ruleFiles.end())
	{
		return ruleFiles[key];
	}
	return nullptr;
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
			ruleFiles[ruleFile->GetID()]= ruleFile;
			ruleFileVector.push_back(ruleFile);
		}
		else
		{
			std::wstring nodename =pugi::as_wide(instructionName);
			nodename.append(L"is another data");
		}
	}
}