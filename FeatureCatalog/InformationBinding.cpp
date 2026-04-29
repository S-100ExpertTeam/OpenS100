#include "stdafx.h"
#include "InformationBinding.h"

#include <algorithm>

InformationBinding::InformationBinding()
{

}

InformationBinding::~InformationBinding()
{

}

void InformationBinding::GetContents(pugi::xml_node& node)
{
	SetRoleType(node.attribute("roleType").value());

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const char* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:multiplicity"))
		{
			multiplicity.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FC:association"))
		{
			association = instruction.attribute("ref").value();
		}
		else if (!strcmp(instructionName, "S100FC:role"))
		{
			role = instruction.attribute("ref").value();
		}
		else if (!strcmp(instructionName, "S100FC:informationType"))
		{
			InsertInformationType(instruction.attribute("ref").value());
		}
	}
}

Multiplicity& InformationBinding::GetMultiplicity()
{
	return multiplicity;
}

std::string InformationBinding::GetAssociation()
{
	return association;
}

std::string InformationBinding::GetAssociationAsWstring()
{
	return association;
}

void InformationBinding::SetAssociation(std::string value)
{
	association = value;
}


std::string InformationBinding::GetRole()
{
	return role;
}

std::string InformationBinding::GetRoleAsWstring()
{
	return role;
}

void InformationBinding::SetRole(std::string value)
{
	role = value;
}


S100_FC_RoleType InformationBinding::GetRoleType()
{
	return roleType;
}

std::string InformationBinding::GetRoleTypeAsString()
{
	if (roleType == S100_FC_RoleType::association)
	{
		return "association";
	}
	else if (roleType == S100_FC_RoleType::aggregation)
	{
		return "aggregation";
	}
	else if (roleType == S100_FC_RoleType::composition)
	{
		return "composition";
	}

	return "";
}

std::string InformationBinding::GetRoleTypeAsWstring()
{
	return GetRoleTypeAsString();
}

void InformationBinding::SetRoleType(S100_FC_RoleType value)
{
	roleType = value;
}

void InformationBinding::SetRoleType(std::string value)
{
	if (value.compare("association") == 0)
	{
		roleType = S100_FC_RoleType::association;
	}
	else if (value.compare("aggregation") == 0)
	{
		roleType = S100_FC_RoleType::aggregation;
	}
	else if (value.compare("composition") == 0)
	{
		roleType = S100_FC_RoleType::composition;
	}
}

int InformationBinding::GetInformationTypeCount()
{
	return (int)informationTypes.size();
}

std::string InformationBinding::GetInformationType(int index)
{
	if (index < 0 || index >= GetInformationTypeCount())
	{
		return "";
	}

	return informationTypes.at(index);
}

std::vector<std::string> InformationBinding::GetInformationTypes()
{
	return informationTypes;
}

std::string InformationBinding::GetInformationTypeAsWstring(int index)
{
	return GetInformationType(index);
}

void InformationBinding::InsertInformationType(std::string value)
{
	informationTypes.push_back(value);
}


bool InformationBinding::IsSameAssociation(InformationBinding& ib)
{
	if (multiplicity == ib.GetMultiplicity() &&
		!association.compare(ib.GetAssociation()) &&
		!role.compare(ib.GetRole()) &&
		roleType == ib.GetRoleType())
	{
		return true;
	}

	return false;
}

void InformationBinding::AppendInformationBinding(InformationBinding& ib)
{
	int cnt = ib.GetInformationTypeCount();
	for (int i = 0; i < cnt; i++)
	{
		InsertInformationType(ib.GetInformationType(i));
	}
}

void InformationBinding::SortInformationType()
{
	std::sort(informationTypes.begin(), informationTypes.end());
}