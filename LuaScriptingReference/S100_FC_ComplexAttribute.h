#pragma once
#include "LuaScriptingReference.h"
#include "S100_FC_Attribute.h"
#include "S100_FC_AttributeBinding.h"

#include "rapidxml.hpp"

class ComplexAttribute;
class KRS_LUA_SCRIPT_REFERENCE_API S100_FC_ComplexAttribute : public S100_FC_Attribute
{

public:
	S100_FC_ComplexAttribute();
	S100_FC_ComplexAttribute(rapidxml::xml_node<>* node);
	S100_FC_ComplexAttribute(ComplexAttribute* node);

public:
	std::vector<S100_FC_AttributeBinding> attributeBinding;
};