#include "stdafx.h"
#include "GF_InformationType.h"

namespace GF
{
	InformationType::InformationType()
	{

	}

	InformationType::InformationType(const InformationType& other) : ObjectType(other)
	{

	}

	InformationType::~InformationType()
	{
	}

	InformationType InformationType::operator=(const InformationType& other)
	{
		ObjectType::operator=(other);

		return *this;
	}

	int InformationType::GetAttributeCount()
	{
		return ObjectType::GetAttributeCount();
	}

}