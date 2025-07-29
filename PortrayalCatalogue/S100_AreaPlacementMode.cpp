#include "stdafx.h"
#include "S100_AreaPlacementMode.h"

S100_AreaPlacementMode StringToS100AreaPlacementMode(const std::string& value)
{
	if (value == "visibleParts")
	{
		return S100_AreaPlacementMode::visibleParts;
	}
	else if (value == "geographic")
	{
		return S100_AreaPlacementMode::geographic;
	}

	return S100_AreaPlacementMode::none; // Default case
}

S100_AreaPlacementMode StringToS100AreaPlacementMode(const std::wstring& value)
{
	return StringToS100AreaPlacementMode(std::string(value.begin(), value.end()));
}
