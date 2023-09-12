#pragma once
#include "LuaScriptingReference.h"

#include <string>
#include <optional>

struct KRS_LUA_SCRIPT_REFERENCE_API point
{
	point() = default;

	std::string x = "";
	std::string y = "";
	std::optional<std::string> z;
};