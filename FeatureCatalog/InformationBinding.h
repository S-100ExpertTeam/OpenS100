#pragma once

#include "Multiplicity.h"
#include "Reference.h"
#include "S100_FC_RoleType.h"

#include <pugixml.hpp>
#include <vector>

class InformationBinding
{
public:
	InformationBinding();
	virtual ~InformationBinding();

private:
	Multiplicity multiplicity;
	std::string association;
	std::string role;
	S100_FC_RoleType roleType = S100_FC_RoleType::none;
	std::vector<std::string> informationTypes;

public:
	void GetContents(pugi::xml_node& node);

	Multiplicity& GetMultiplicity();

	std::string GetAssociation();
	std::string GetAssociationAsWstring();

	void SetAssociation(std::string value);

	std::string GetRole();
	std::string GetRoleAsWstring();

	void SetRole(std::string value);

	S100_FC_RoleType GetRoleType();
	std::string GetRoleTypeAsString();
	std::string GetRoleTypeAsWstring();

	void SetRoleType(S100_FC_RoleType value);
	void SetRoleType(std::string value);

	int GetInformationTypeCount();

	std::string GetInformationType(int index);
	std::vector<std::string> GetInformationTypes();
	std::string GetInformationTypeAsWstring(int index);

	void InsertInformationType(std::string value);
	void InsertInformationType(std::string value);

	bool IsSameAssociation(InformationBinding& ib);
	void AppendInformationBinding(InformationBinding& ib);
	
	void SortInformationType();
};