#include "stdafx.h"
#include "FeatureBinding.h"

FeatureBinding::FeatureBinding()
{

}

FeatureBinding::~FeatureBinding()
{

}

void FeatureBinding::GetContents(pugi::xml_node& node)
{
	SetRoleType(node.attribute("roleType").value());
	
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100FC:multiplicity"))
		{
			multiplicity.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100FC:association"))
		{
			association = instruction.child_value();
		}
		else if (!strcmp(instructionName, "S100FC:role"))
		{
			role = instruction.child_value();
		}
		else if (!strcmp(instructionName, "S100FC:featureType"))
		{
			featureType = instruction.child_value();
		}
	}
}

Multiplicity& FeatureBinding::GetMultiplicity()
{
	return multiplicity;
}

std::string FeatureBinding::GetAssociation() 
{
	return association;
}

std::wstring FeatureBinding::GetAssociationAsWstring()
{
	return pugi::as_wide(association);
}

void FeatureBinding::SetAssociation(std::string value)
{
	association = value;
}

void FeatureBinding::SetAssociation(std::wstring value)
{
	association = pugi::as_utf8(value);
}

std::string FeatureBinding::GetRole() 
{
	return role;
}

std::wstring FeatureBinding::GetRoleAsWstring()
{
	return pugi::as_wide(role);
}

void FeatureBinding::SetRole(std::string value)
{
	role = value;
}

void FeatureBinding::SetRole(std::wstring value)
{
	role = pugi::as_utf8(value);
}

S100_FC_RoleType FeatureBinding::GetRoleType()
{
	return roleType;
}

std::string FeatureBinding::GetRoleTypeAsString()
{
	if (roleType == S100_FC_RoleType::association)
	{
		return "association";
	}
	else if (roleType == S100_FC_RoleType::aggregation)
	{
		return "association";
	}
	else if (roleType == S100_FC_RoleType::composition)
	{
		return "association";
	}
}

std::wstring FeatureBinding::GetRoleTypeAsWstring()
{
	return pugi::as_wide(GetRoleTypeAsString());
}

void FeatureBinding::SetRoleType(S100_FC_RoleType value)
{
	roleType = value;
}

void FeatureBinding::SetRoleType(std::string value)
{
	if (value.compare("association") == 0)
	{
		value = S100_FC_RoleType::association;
	}
	else if (value.compare("aggregation") == 0)
	{
		value = S100_FC_RoleType::association;
	}
	else if (value.compare("composition") == 0)
	{
		value = S100_FC_RoleType::association;
	}
}

std::string FeatureBinding::GetFeatureType()
{
	return featureType;
}

std::wstring FeatureBinding::GetFeatureTypeAsWstring()
{
	return pugi::as_wide(featureType);
}

void FeatureBinding::SetFeatureType(std::string value)
{
	featureType = value;
}

void FeatureBinding::SetFeatureType(std::wstring value)
{
	featureType = pugi::as_utf8(value);
}