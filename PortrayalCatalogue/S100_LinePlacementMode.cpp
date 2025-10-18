#include "stdafx.h"
#include "S100_LinePlacementMode.h"

S100_LinePlacementMode StringToLinePlacementMode(const std::string& value)
{
	if (value == "relative")
	{
		return S100_LinePlacementMode::relative;
	}
	else if (value == "absolute")
	{
		return S100_LinePlacementMode::absolute;
	}

	return S100_LinePlacementMode::none;
}
