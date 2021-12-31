#pragma once
#include "LuaScriptingReference.h"
#include "S100_FC_ObjectType.h"
#include "rapidxml.hpp"
#include "S100_FC_ItemReference.h"

#include <vector>

class InformationType;

class KRS_LUA_SCRIPT_REFERENCE_API S100_FC_InformationType : public S100_FC_ObjectType
{
public:
	S100_FC_InformationType();
	S100_FC_InformationType(rapidxml::xml_node<>* node);
	S100_FC_InformationType(InformationType* node);

public:
	S100_FC_ItemReference<S100_FC_InformationType> superType;
	std::vector<S100_FC_ItemReference<S100_FC_InformationType>> subType;
};