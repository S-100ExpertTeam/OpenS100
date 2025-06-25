#include "stdafx.h"
#include "GF_SimpleAttributeType.h"
#include "ProcessS101.h"

namespace GF
{
	SimpleAttributeType::SimpleAttributeType()
	{

	}

	SimpleAttributeType::SimpleAttributeType(const SimpleAttributeType& other) : ThematicAttributeType(other)
	{
		valueType = other.valueType;
		value = other.value;
	}

	SimpleAttributeType::SimpleAttributeType(FCD::S100_CD_AttributeValueType valueType, std::string value)
	{
		this->valueType = valueType;
		this->value = value;
	}

	SimpleAttributeType::~SimpleAttributeType()
	{

	}

	std::string SimpleAttributeType::GetValue()
	{
		if (value.empty()) {
			return ProcessS101::g_unknown_attribute_value;
		}
		return value; 
	}
	
	bool SimpleAttributeType::IsSimple()
	{
		return true; 
	}

	ThematicAttributeType* SimpleAttributeType::clone()
	{
		return new SimpleAttributeType(*this);
	}
}