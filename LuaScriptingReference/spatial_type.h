#pragma once
#include "LuaScriptingReference.h"

enum class KRS_LUA_SCRIPT_REFERENCE_API spatial_type
{
	unknown,
	point,
	multi_point,
	curve,
	composite_curve,
	surface
};