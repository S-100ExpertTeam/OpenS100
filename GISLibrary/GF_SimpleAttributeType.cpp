#include "stdafx.h"
#include "GF_SimpleAttributeType.h"

namespace GF
{
	SimpleAttributeType::SimpleAttributeType()
	{

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
		return value; 
	}
	
	bool SimpleAttributeType::IsSimple()
	{
		return true; 
	}
}