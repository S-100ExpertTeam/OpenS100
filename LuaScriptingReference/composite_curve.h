#pragma once
#include "LuaScriptingReference.h"
#include "spatial_association.h"

#include <vector>

struct KRS_LUA_SCRIPT_REFERENCE_API composite_curve
{
	std::vector<spatial_association> curve_associations;
};