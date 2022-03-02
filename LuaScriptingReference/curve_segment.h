#pragma once
#include "LuaScriptingReference.h"
#include "point.h"

#include <vector>
#include <string>

struct KRS_LUA_SCRIPT_REFERENCE_API curve_segment
{
	std::vector<point> control_points;
	std::string interpolation;
};