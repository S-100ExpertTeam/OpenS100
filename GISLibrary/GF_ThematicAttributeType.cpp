#include "stdafx.h"
#include "GF_ThematicAttributeType.h"

namespace GF
{

	ThematicAttributeType::ThematicAttributeType()
	{

	}

	ThematicAttributeType::~ThematicAttributeType()
	{
		
	}

	ThematicAttributeType* ThematicAttributeType::clone()
	{
		return new ThematicAttributeType(*this);
	}
}