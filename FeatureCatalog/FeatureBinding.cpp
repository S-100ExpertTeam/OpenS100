#include "stdafx.h"
#include "FeatureBinding.h"

#include <algorithm>

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
		else if (!strcmp(instructionName, "S100FC:featureType"))
		{
			InsertFeatureType(instruction.attribute("ref").value());
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

std::string FeatureBinding::GetAssociationAsWstring()
{
	return association;
}

void FeatureBinding::SetAssociation(std::string value)
{
	association = value;
}


std::string FeatureBinding::GetRole() 
{
	return role;
}

std::string FeatureBinding::GetRoleAsWstring()
{
	return role;
}

void FeatureBinding::SetRole(std::string value)
{
	role = value;
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
		return "aggregation";
	}
	else if (roleType == S100_FC_RoleType::composition)
	{
		return "composition";
	}

	return "";
}

std::string FeatureBinding::GetRoleTypeAsWstring()
{
	return GetRoleTypeAsString();
}

void FeatureBinding::SetRoleType(S100_FC_RoleType value)
{
	roleType = value;
}

void FeatureBinding::SetRoleType(std::string value)
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

int FeatureBinding::GetFeatureTypeCount()
{
	return (int)featureTypes.size();
}

std::string FeatureBinding::GetFeatureType(int index)
{
	if (index < 0 || index >= GetFeatureTypeCount())
	{
		return "";
	}

	return featureTypes.at(index);
}

std::vector<std::string> FeatureBinding::GetFeatureTypes()
{
	return featureTypes;
}

std::string FeatureBinding::GetFeatureTypeAsWstring(int index)
{
	return GetFeatureType(index);
}

void FeatureBinding::InsertFeatureType(std::string value)
{
	featureTypes.push_back(value);
}


bool FeatureBinding::IsSameAssociation(FeatureBinding& fb)
{
	if (multiplicity == fb.GetMultiplicity() &&
		!association.compare(fb.GetAssociation()) &&
		!role.compare(fb.GetRole()) &&
		roleType == fb.GetRoleType())
	{
		return true;
	}

	return false;
}

void FeatureBinding::AppendFeatureBinding(FeatureBinding& fb)
{
	int cnt = fb.GetFeatureTypeCount();
	for (int i = 0; i < cnt; i++)
	{
		InsertFeatureType(fb.GetFeatureType(i));
	}
}

void FeatureBinding::SortFeatureType()
{
	std::sort(featureTypes.begin(), featureTypes.end());
}