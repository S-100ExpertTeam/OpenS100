#pragma once

#include "Multiplicity.h"
#include "Reference.h"
#include "S100_FC_RoleType.h"

class FeatureBinding
{
public:
	FeatureBinding();
	virtual ~FeatureBinding();

private:
	Multiplicity multiplicity;
	std::string association;
	std::string role;
	S100_FC_RoleType roleType = S100_FC_RoleType::none;
	std::string featureType;

public:
	void GetContents(pugi::xml_node& node);

	Multiplicity& GetMultiplicity();

	std::string GetAssociation();
	std::wstring GetAssociationAsWstring();

	void SetAssociation(std::string value);
	void SetAssociation(std::wstring value);

	std::string GetRole();
	std::wstring GetRoleAsWstring();

	void SetRole(std::string value);
	void SetRole(std::wstring value);

	S100_FC_RoleType GetRoleType();
	std::string GetRoleTypeAsString();
	std::wstring GetRoleTypeAsWstring();

	void SetRoleType(S100_FC_RoleType value);
	void SetRoleType(std::string value);

	std::string GetFeatureType();
	std::wstring GetFeatureTypeAsWstring();

	void SetFeatureType(std::string value);
	void SetFeatureType(std::wstring value);
};