#pragma once
#include "LuaScriptingReference.h"
#include "rapidxml.hpp"

#include <string>
#include <optional>
#include <vector>

class Item;
class KRS_LUA_SCRIPT_REFERENCE_API S100_FC_Item
{
public:
	S100_FC_Item();
	S100_FC_Item(rapidxml::xml_node<>* node);
	S100_FC_Item(Item* node);

public:
	std::string name;
	std::string definition;
	std::string code;
	std::optional<std::string> remarks;
	std::vector<std::string> alias;
};