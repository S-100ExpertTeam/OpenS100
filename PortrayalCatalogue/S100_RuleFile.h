#pragma once
#include "..\\extlibs\pugixml\include\pugixml.hpp"

#include <string>

class S100_RuleFile
{
public:
	S100_RuleFile();
	virtual ~S100_RuleFile();
	
private:
	std::string id = "";
	std::wstring fileName = L"";
	std::wstring fileType = L"";
	std::wstring fileFormat = L"";
	std::wstring ruleType = L"";

public:
	void GetContents(pugi::xml_node& node);

	void SetID(std::string& value);
	std::string GetID();

	void SetFileName(std::wstring& value);
	std::wstring GetFileName();

	void SetFileType(std::wstring& value);
	std::wstring GetFileType();

	void SetFileFormat(std::wstring& value);
	std::wstring GetFileFormat();

	void SetRuleType(std::wstring& value);
	std::wstring GetRuleType();

	bool IsTypeLevelTemplate();
	
	bool IsLua();
};