#pragma once

#include "LuaScriptingReference.h"
#include "point.h"

#include <vector>

struct KRS_LUA_SCRIPT_REFERENCE_API multipoint
{
	std::vector<point> points;
};