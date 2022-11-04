#pragma once
#include "LuaScriptingReference.h"

#include <string>

class KRS_LUA_SCRIPT_REFERENCE_API context_parameter
{
public:
	context_parameter();
	context_parameter(std::string_view name, std::string_view type, std::string_view default_value);
	virtual ~context_parameter();

public:
	std::string name;
	std::string type;
	std::string default_value;
}; 