#pragma once
#include "LuaScriptingReference.h"

#include <map>

enum class KRS_LUA_SCRIPT_REFERENCE_API S100_CD_FeatureUseType
{
	unknown,
	geographic,
	meta,
	cartographic,
	theme
};

extern KRS_LUA_SCRIPT_REFERENCE_API std::map<S100_CD_FeatureUseType, std::string> S100_CD_FeatureUseType_Names;