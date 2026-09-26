#include "stdafx.h"
#include "S100_AreaFills.h"


namespace Portrayal
{
	S100_AreaFills::S100_AreaFills()
	{

	}

	S100_AreaFills::~S100_AreaFills()
	{
		for (auto itor = areaFillFiles.begin(); itor != areaFillFiles.end(); itor++)
		{
			delete itor->second;
		}
	}

	void S100_AreaFills::GetContents(pugi::xml_node& node)
	{
		for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			const pugi::char_t* instructionName = instruction.name();
			if (!strcmp(instructionName, "areaFill"))
			{
				ExternalFile* areaFillFIle = new ExternalFile();
				areaFillFIle->GetContents(instruction);
				areaFillFiles[areaFillFIle->GetId()] = areaFillFIle;
				areaFillFiles_v.push_back(areaFillFIle);
			}
		}
	}

	void S100_AreaFills::SetAreaFillFiles(const std::string& key, ExternalFile* value)  { areaFillFiles.insert({ key, value }); }
	void S100_AreaFills::SetAreaFillFiles(const std::wstring& key, ExternalFile* value) { areaFillFiles.insert({ toUtf8(key), value }); }

	void S100_AreaFills::SetAreaFillFiles(std::unordered_map<std::string, ExternalFile*> value)
	{
		areaFillFiles = value;
	}

	ExternalFile* S100_AreaFills::GetAreaFillFiles(const std::string& value)
	{
		auto it = areaFillFiles.find(value);
		return (it != areaFillFiles.end()) ? it->second : nullptr;
	}

	ExternalFile* S100_AreaFills::GetAreaFillFiles(const std::wstring& value)
	{
		return GetAreaFillFiles(toUtf8(value));
	}

	std::unordered_map<std::string, ExternalFile*> S100_AreaFills::GetAreaFillFiles()
	{
		return areaFillFiles;
	}

	std::vector<ExternalFile*>* S100_AreaFills::GetAreaFillFilesVecter()
	{
		return &areaFillFiles_v;
	}
}
