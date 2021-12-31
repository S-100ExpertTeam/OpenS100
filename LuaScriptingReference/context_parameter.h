#pragma once
#include "LuaScriptingReference.h"

#include <string>

struct KRS_LUA_SCRIPT_REFERENCE_API context_parameter
{
	std::string name;
	std::string type;
	std::string default_value;
}; 