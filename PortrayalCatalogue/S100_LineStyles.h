#pragma once
#include "S100_LineStyleFile.h"

#include <unordered_map>
#include <string>

class S100_LineStyles
{
public:
	S100_LineStyles();
	virtual ~S100_LineStyles();

private:
	std::unordered_map<std::wstring, S100_LineStyleFile*> lineStyleFiles;

public:
	void SetLineStyleFiles(std::wstring& key, S100_LineStyleFile* value);
	S100_LineStyleFile* GetLineStyleFiles(std::wstring key);
	std::unordered_map<std::wstring, S100_LineStyleFile*> GetLineStyleFiles();

	void GetContents(pugi::xml_node& node);
};