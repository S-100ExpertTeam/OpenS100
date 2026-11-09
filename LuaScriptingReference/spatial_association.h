#pragma once
#include "LuaScriptingReference.h"

#include <string>
#include <optional>

struct KRS_LUA_SCRIPT_REFERENCE_API spatial_association
{
	std::string spatial_type;
	std::string spatial_id;
	std::string orientation;
	std::optional<int> scale_minimum;
	std::optional<int> scale_maximum;
};