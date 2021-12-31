#pragma once
#include "LuaScriptingReference.h"
#include "rapidxml.hpp"
#include "S100_FC_Item.h"

class Role;
class KRS_LUA_SCRIPT_REFERENCE_API S100_FC_Role : public S100_FC_Item
{
public:
	S100_FC_Role();
	S100_FC_Role(rapidxml::xml_node<>* node);
	S100_FC_Role(Role* a);
};