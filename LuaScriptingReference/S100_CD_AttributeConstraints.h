#pragma once
#include "LuaScriptingReference.h"
#include "S100_NumericRange.h"
#include "rapidxml.hpp"

#include <string>
#include <optional>

class KRS_LUA_SCRIPT_REFERENCE_API S100_CD_AttributeConstraints
{
public:
	S100_CD_AttributeConstraints();
	S100_CD_AttributeConstraints(rapidxml::xml_node<>* node);

public:
	std::optional<int> stringLength;
	std::optional<std::string> textPattern;
	std::optional<S100_NumericRange> range;
	std::optional<int> precision;
};