#pragma once
#include "LuaScriptingReference.h"
#include "S100_FC_Item.h"

class Attribute;
class KRS_LUA_SCRIPT_REFERENCE_API S100_FC_Attribute : public S100_FC_Item
{
public:
	S100_FC_Attribute();
	S100_FC_Attribute(rapidxml::xml_node<>* node);
	S100_FC_Attribute(Attribute* a);
};