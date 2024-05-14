#include "stdafx.h"
#include "GF_ThematicAttributeType.h"

namespace GF
{

	ThematicAttributeType::ThematicAttributeType()
	{

	}

	ThematicAttributeType::ThematicAttributeType(const ThematicAttributeType& other) :AttributeType(other)
	{

	}

	ThematicAttributeType::~ThematicAttributeType()
	{

	}

	ThematicAttributeType ThematicAttributeType::operator=(const ThematicAttributeType& other)
	{
		AttributeType::operator=(other);

		return *this;
	}

	ThematicAttributeType* ThematicAttributeType::clone()
	{
		return new ThematicAttributeType(*this);
	}

}