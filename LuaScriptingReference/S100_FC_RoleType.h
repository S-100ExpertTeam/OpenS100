#pragma once
#include "LuaScriptingReference.h"

#include <map>

enum class KRS_LUA_SCRIPT_REFERENCE_API S100_FC_RoleType
{
	unknown,
	association,
	aggregation,
	composition
};

extern KRS_LUA_SCRIPT_REFERENCE_API std::map<S100_FC_RoleType, std::string> S100_FC_RoleType_Names;