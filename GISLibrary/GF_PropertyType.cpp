#include "stdafx.h"
#include "GF_PropertyType.h"

namespace GF
{
	PropertyType::PropertyType()
	{

	}

	PropertyType::PropertyType(const PropertyType& other)
	{
		code = other.code;
	}

	PropertyType::~PropertyType()
	{

	}

	std::string PropertyType::GetCode()
	{
		return code;
	}

	void PropertyType::SetCode(std::string value)
	{
		code = value;
	}
}