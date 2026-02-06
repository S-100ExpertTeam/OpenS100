#include "stdafx.h"
#include "S100_LinePlacementMode.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

S100_LinePlacementMode StringToLinePlacementMode(const std::string& value)
{
	auto lower = LatLonUtility::To_Lowercase(value);

	if (lower == "relative")
	{
		return S100_LinePlacementMode::relative;
	}
	else if (lower == "absolute")
	{
		return S100_LinePlacementMode::absolute;
	}

	return S100_LinePlacementMode::none;
}

S100_LinePlacementMode StringToLinePlacementMode(const std::wstring& value)
{
	return StringToLinePlacementMode(std::string(value.begin(), value.end()));
}