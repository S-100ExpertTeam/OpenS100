#pragma once

#include <string>

enum class S100_LinePlacementMode
{
	none = 0,
	relative = 1,
	absolute = 2
};

S100_LinePlacementMode StringToLinePlacementMode(const std::string& value);