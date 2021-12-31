#pragma once
#include "LuaScriptingReference.h"

#include <map>

enum class KRS_LUA_SCRIPT_REFERENCE_API S100_IntervalType
{
	unknown,
	openInterval,
	geLtInterval,
	gtLeInterval,
	closedInterval,
	gtSemiInterval,
	geSemiInterval,
	ltSemiInterval,
	leSemiInterval
};

extern KRS_LUA_SCRIPT_REFERENCE_API std::map<S100_IntervalType, std::string> S100_IntervalType_Names;