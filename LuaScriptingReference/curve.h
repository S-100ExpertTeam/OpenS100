#pragma once
#include "LuaScriptingReference.h"
#include "spatial_association.h"
#include "curve_segment.h"

#include<vector>

struct KRS_LUA_SCRIPT_REFERENCE_API curve
{
	spatial_association start_point;
	spatial_association end_point;
	std::vector<curve_segment> segments;
};