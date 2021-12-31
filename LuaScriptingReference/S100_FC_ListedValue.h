#pragma once
#include "LuaScriptingReference.h"
#include "rapidxml.hpp"

#include <string>
#include <optional>
#include <vector>

class ListedValue;
class KRS_LUA_SCRIPT_REFERENCE_API S100_FC_ListedValue
{
public:
	S100_FC_ListedValue();
	S100_FC_ListedValue(rapidxml::xml_node<>* node);
	S100_FC_ListedValue(ListedValue* a);

public:
	std::string label;
	std::string definition;
	int code;
	std::optional<std::string> remarks;
	std::vector<std::string> alias;
};