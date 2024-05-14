#include "stdafx.h"
#include "GF_AssociationRole.h"

namespace GF
{

	AssociationRole::AssociationRole()
	{

	}

	AssociationRole::~AssociationRole()
	{

	}

	AssociationRole AssociationRole::operator=(const AssociationRole& other)
	{
		PropertyType::operator=(other);

		return *this;
	}
}