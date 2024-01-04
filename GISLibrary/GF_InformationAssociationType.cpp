#include "stdafx.h"
#include "GF_InformationAssociationType.h"

namespace GF
{
	InformationAssociationType::InformationAssociationType()
	{

	}

	InformationAssociationType::InformationAssociationType(const InformationAssociationType& other) :AssociationType(other)
	{
		informationID = other.informationID;
	}

	InformationAssociationType::~InformationAssociationType()
	{

	}

	InformationAssociationType InformationAssociationType::operator=(const InformationAssociationType& other)
	{
		AssociationType::operator=(other);

		informationID = other.informationID;

		return *this;
	}

	std::string InformationAssociationType::GetInformationID()
	{
		return informationID;
	}

	void InformationAssociationType::SetInformationID(std::string value)
	{
		informationID = value;
	}
}