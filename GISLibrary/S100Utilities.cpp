#include "stdafx.h"
#include "S100Utilities.h"

#include <string>
#include <filesystem>
#include <cctype>

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