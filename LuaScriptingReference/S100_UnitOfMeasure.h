#pragma once
#include "LuaScriptingReference.h"
#include "rapidxml.hpp"

#include <optional>
#include <string>

class UnitOfMeasure;
class KRS_LUA_SCRIPT_REFERENCE_API S100_UnitOfMeasure
{
public:
	S100_UnitOfMeasure();
	S100_UnitOfMeasure(rapidxml::xml_node<>* node);
	S100_UnitOfMeasure(UnitOfMeasure* uom);

public:
	std::string name;
	std::optional<std::string> definition;
	std::optional<std::string> symbol;
};