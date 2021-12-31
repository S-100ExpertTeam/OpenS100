#pragma once
#include "LuaScriptingReference.h"
#include "S100_FC_Attribute.h"
#include "S100_CD_AttributeValueType.h"
#include "S100_UnitOfMeasure.h"
#include "S100_CD_QuantitySpecification.h"
#include "S100_CD_AttributeConstraints.h"
#include "S100_FC_ListedValue.h"

class SimpleAttribute;
class KRS_LUA_SCRIPT_REFERENCE_API S100_FC_SimpleAttribute : public S100_FC_Attribute
{
public:
	S100_FC_SimpleAttribute();
	S100_FC_SimpleAttribute(rapidxml::xml_node<>* node);// : S100_FC_Attribute(node) {};
	S100_FC_SimpleAttribute(SimpleAttribute* sa);

public:
	S100_CD_AttributeValueType valueType;
	std::optional<S100_UnitOfMeasure> uom;
	std::optional<S100_CD_QuantitySpecification> quantitySpecification;
	std::optional<S100_CD_AttributeConstraints> constraints;
	std::vector<S100_FC_ListedValue> listedValue;
};