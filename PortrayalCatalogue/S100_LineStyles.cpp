#include "stdafx.h"
#include "S100_LineStyles.h"


namespace Portrayal
{
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
				ExternalFile* lineStyle = new ExternalFile();
				lineStyle->GetContents(instruction);
				lineStyleFiles[lineStyle->GetId()] = lineStyle;
				lineStyleFiles_v.push_back(lineStyle);
			}
		}
	}

	void S100_LineStyles::SetLineStyleFiles(std::wstring& key, ExternalFile* value)
	{
		lineStyleFiles.insert({ key, value });
	}

	ExternalFile* S100_LineStyles::GetLineStyleFiles(std::wstring key)
	{
		auto isline = lineStyleFiles.find(key);
		if (isline != lineStyleFiles.end())
		{
			return lineStyleFiles[key];
		}
		return nullptr;
	}

	std::unordered_map<std::wstring, ExternalFile*> S100_LineStyles::GetLineStyleFiles()
	{
		return lineStyleFiles;
	}


	std::vector< ExternalFile*>* S100_LineStyles::GetLineStyleFilesVector()
	{
		return &lineStyleFiles_v;
	}
}