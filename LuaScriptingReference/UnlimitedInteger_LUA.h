#pragma once
#include "LuaScriptingReference.h"
#include "rapidxml.hpp"

#include <optional>

class Multiplicity;

class KRS_LUA_SCRIPT_REFERENCE_API UnlimitedInteger_LUA
{
public:
	UnlimitedInteger_LUA();
	UnlimitedInteger_LUA(rapidxml::xml_node<>* node);
	UnlimitedInteger_LUA(Multiplicity* node);

public:
	bool infinite;
	std::optional<int> value;
};