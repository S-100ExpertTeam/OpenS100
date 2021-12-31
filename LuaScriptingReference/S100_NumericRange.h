#pragma once
#include "LuaScriptingReference.h"
#include "S100_IntervalType.h"
#include "rapidxml.hpp"

#include <optional>

class KRS_LUA_SCRIPT_REFERENCE_API S100_NumericRange
{
public:
	S100_NumericRange();
	S100_NumericRange(rapidxml::xml_node<>* node);

public:
	std::optional<double> lower;
	std::optional<double> upper;
	S100_IntervalType closure;
};