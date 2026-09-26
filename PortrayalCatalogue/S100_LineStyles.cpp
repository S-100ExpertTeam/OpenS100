#include "stdafx.h"
#include "S100_LineStyles.h"

#include <string>

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
			std::string instructionName = instruction.name();
			if (instructionName.find("lineStyle") != std::string::npos)
			{
				ExternalFile* lineStyle = new ExternalFile();
				lineStyle->GetContents(instruction);
				lineStyleFiles[lineStyle->GetId()] = lineStyle;
				lineStyleFiles_v.push_back(lineStyle);
			}
		}
	}

	void S100_LineStyles::SetLineStyleFiles(const std::string& key, ExternalFile* value)  { lineStyleFiles.insert({ key, value }); }
	void S100_LineStyles::SetLineStyleFiles(const std::wstring& key, ExternalFile* value) { lineStyleFiles.insert({ toUtf8(key), value }); }

	ExternalFile* S100_LineStyles::GetLineStyleFiles(const std::string& key)
	{
		auto it = lineStyleFiles.find(key);
		return (it != lineStyleFiles.end()) ? it->second : nullptr;
	}

	ExternalFile* S100_LineStyles::GetLineStyleFiles(const std::wstring& key)
	{
		return GetLineStyleFiles(toUtf8(key));
	}

	std::unordered_map<std::string, ExternalFile*> S100_LineStyles::GetLineStyleFiles()
	{
		return lineStyleFiles;
	}

	std::vector<ExternalFile*>* S100_LineStyles::GetLineStyleFilesVector()
	{
		return &lineStyleFiles_v;
	}
}
