#pragma once
#include "S100_FC_Item.h"
#include "S100_FC_AttributeBinding.h"
#include "LuaScriptingReference.h"

#include "rapidxml.hpp"

class NamedType;
class KRS_LUA_SCRIPT_REFERENCE_API S100_FC_NamedType : public S100_FC_Item
{
public:
	S100_FC_NamedType();
	S100_FC_NamedType(rapidxml::xml_node<>* node);
	S100_FC_NamedType(NamedType* node);

public:
	bool isAbstract;
	std::vector<S100_FC_AttributeBinding> attributeBinding;
};