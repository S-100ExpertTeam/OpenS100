#include "stdafx.h"
#include "GF_InformationAssociationType.h"

namespace GF
{
	InformationAssociationType::InformationAssociationType()
	{

	}

	InformationAssociationType::~InformationAssociationType()
	{

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