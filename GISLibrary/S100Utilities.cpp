#include "stdafx.h"
#include "S100Utilities.h"

#include <string>
#include <filesystem>
#include <cctype>

bool S100Utilities::IsS100Catalog(std::wstring path)
{
	pugi::xml_document xmldoc;
	xmldoc.load_file(path.c_str());
	auto name = xmldoc.first_child().name();
	if (strcmp("S100XC:S100_ExchangeCatalogue",name)==0) 
	{
		return true;
	}
	return false;
}

int S100Utilities::GetLevel(std::wstring path)
{
	// get without extension
	std::wstring name = std::filesystem::path(path).stem();
	if (name.length() >= 3)
	{
		auto levelStr = name.substr(7, 1);
		if (std::isdigit(levelStr.at(0)))
		{
			auto level = std::stoi(levelStr);
			return level;
		}
	}

	return 0;
}

int S100Utilities::GetScaleByLevel(int level)
{
	return 0;
}