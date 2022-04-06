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

	void S100_AreaFills::SetAreaFillFiles(std::wstring key, ExternalFile* value)
	{
		areaFillFiles.insert({ key,value });
	}
	void S100_AreaFills::SetAreaFillFiles(std::unordered_map<std::wstring, ExternalFile*> value)
	{
		areaFillFiles = value;
	}

	ExternalFile* S100_AreaFills::GetAreaFillFiles(std::wstring value)
	{
		auto isarea = areaFillFiles.find(value);
		if (isarea != areaFillFiles.end())
		{
			return areaFillFiles[value];
		}
		return nullptr;
	}

	std::unordered_map<std::wstring, ExternalFile*> S100_AreaFills::GetAreaFillFiles()
	{
		return areaFillFiles;
	}


	std::vector< ExternalFile*>* S100_AreaFills::GetAreaFillFilesVecter()
	{
		return &areaFillFiles_v;
	}
}