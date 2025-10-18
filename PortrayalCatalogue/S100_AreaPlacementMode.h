#pragma once

#include <string>

enum class S100_AreaPlacementMode
{
	none = 0,
	visibleParts,
	geographic
};

S100_AreaPlacementMode StringToS100AreaPlacementMode(const std::string& value);
S100_AreaPlacementMode StringToS100AreaPlacementMode(const std::wstring& value);