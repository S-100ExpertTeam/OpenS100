#pragma once
#include "LuaScriptingReference.h"

#include <map>

enum class KRS_LUA_SCRIPT_REFERENCE_API S100_FC_SpatialPrimitiveType
{
	unknown,
	point,
	pointSet,
	curve,
	surface,
	coverage,
	noGeometry
};

extern KRS_LUA_SCRIPT_REFERENCE_API std::map<S100_FC_SpatialPrimitiveType, std::string> S100_FC_SpatialPrimitiveType_Names;