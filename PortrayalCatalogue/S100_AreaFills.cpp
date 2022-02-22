#include "stdafx.h"

#include "S100_AreaFills.h"

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
			S100_AreaFillFile* areaFillFIle = new S100_AreaFillFile();
			areaFillFIle->GetContents(instruction);
			areaFillFiles[areaFillFIle->Getid()]= areaFillFIle;
		}
	}
}

void S100_AreaFills::SetAreaFillFiles(std::wstring key, S100_AreaFillFile* value)
{
	areaFillFiles.insert({key,value});
}
void S100_AreaFills::SetAreaFillFiles(std::unordered_map<std::wstring, S100_AreaFillFile*> value)
{
	areaFillFiles = value;
}

S100_AreaFillFile* S100_AreaFills::GetAreaFillFiles(std::wstring value)
{
	auto isarea = areaFillFiles.find(value);
	if (isarea!=areaFillFiles.end())
	{
		return areaFillFiles[value];
	}
	return nullptr;
}

std::unordered_map<std::wstring, S100_AreaFillFile*> S100_AreaFills::GetAreaFillFiles()
{
	return areaFillFiles;
}