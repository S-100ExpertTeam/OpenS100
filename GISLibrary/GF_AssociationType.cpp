#include "stdafx.h"
#include "GF_AssociationType.h"

namespace GF
{
	AssociationType::AssociationType()
	{

	}

	AssociationType::AssociationType(const AssociationType& other) : NamedType(other)
	{
		role = other.role;
	}

	AssociationType::~AssociationType()
	{

	}

	AssociationType AssociationType::operator=(const AssociationType& other)
	{
		NamedType::operator=(other);

		role = other.role;

		return *this;
	}

	std::string AssociationType::GetRole()
	{
		return role.GetCode();
	}

	void AssociationType::SetRole(std::string code)
	{
		role.SetCode(code);
	}

}