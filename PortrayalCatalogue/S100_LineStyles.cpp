#include "stdafx.h"
#include "S100_LineStyles.h"

S100_LineStyles::S100_LineStyles()
{

}

S100_LineStyles::~S100_LineStyles()
{
	for (auto itor = lineStyleFiles.begin(); itor != lineStyleFiles.end(); itor++)
	{
		delete itor->second;
	}
}

void S100_LineStyles::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "lineStyle"))
		{
			S100_LineStyleFile* lineStyle = new S100_LineStyleFile();
			lineStyle->GetContents(instruction);
			lineStyleFiles[lineStyle->GetId()]= lineStyle;
		}
	}
}

void S100_LineStyles::SetLineStyleFiles(std::wstring& key, S100_LineStyleFile* value) 
{
	lineStyleFiles.insert({ key, value });
}

S100_LineStyleFile* S100_LineStyles::GetLineStyleFiles(std::wstring key) 
{
	auto isline = lineStyleFiles.find(key);
	if (isline!= lineStyleFiles.end())
	{
		return lineStyleFiles[key];
	}
	return nullptr;
}

std::unordered_map<std::wstring, S100_LineStyleFile*> S100_LineStyles::GetLineStyleFiles()
{
	return lineStyleFiles;
} 