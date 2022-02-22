#pragma once
#include "S100_AreaFillFile.h"

#include <unordered_map>
#include <string>

class S100_AreaFills 
{
public:
	S100_AreaFills();
	virtual ~S100_AreaFills();

private:
	std::unordered_map<std::wstring, S100_AreaFillFile*> areaFillFiles;

public:
	void GetContents(pugi::xml_node& node);

	void SetAreaFillFiles(std::wstring key, S100_AreaFillFile* value);
	void SetAreaFillFiles(std::unordered_map<std::wstring, S100_AreaFillFile*> value);

	S100_AreaFillFile* GetAreaFillFiles(std::wstring value);
	std::unordered_map<std::wstring, S100_AreaFillFile*> GetAreaFillFiles();
};