#pragma once
#include "LuaScriptingReference.h"
#include "S100_FC_NamedType.h"
#include "S100_FC_InformationBinding.h"

class S100ObjectType;
class KRS_LUA_SCRIPT_REFERENCE_API S100_FC_ObjectType : public S100_FC_NamedType
{
public:
	S100_FC_ObjectType();
	S100_FC_ObjectType(rapidxml::xml_node<>* node);
	S100_FC_ObjectType(S100ObjectType* a);

public:
	std::vector<S100_FC_InformationBinding> informationBinding;
};