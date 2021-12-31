#pragma once
#include "LuaScriptingReference.h"
#include "rapidxml.hpp"

#include "S100_Multiplicity.h"
#include "S100_FC_ItemReference.h"
#include "S100_FC_Attribute.h"
#include <vector>

class AttributeBinding;
class KRS_LUA_SCRIPT_REFERENCE_API S100_FC_AttributeBinding
{
public:
	S100_FC_AttributeBinding();
	S100_FC_AttributeBinding(rapidxml::xml_node<>* node);
	S100_FC_AttributeBinding(AttributeBinding* node);

	S100_Multiplicity multiplicity;
	bool sequential;
	S100_FC_ItemReference<S100_FC_Attribute> attribute;
	std::vector<int> permittedValues;
};