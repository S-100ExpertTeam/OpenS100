#pragma once

#include "Multiplicity.h"
#include "Reference.h"
#include "S100_FC_RoleType.h"

#include <pugixml.hpp>
#include <vector>

class FeatureBinding
{
public:
	FeatureBinding();
	virtual ~FeatureBinding();

protected:
	Multiplicity multiplicity;
	std::string association;
	std::string role;
	S100_FC_RoleType roleType = S100_FC_RoleType::none;
	std::vector<std::string> featureTypes;

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

	int GetFeatureTypeCount();

	std::string GetFeatureType(int index);
	std::vector<std::string> GetFeatureTypes();
	std::wstring GetFeatureTypeAsWstring(int index);

	void InsertFeatureType(std::string value);
	void InsertFeatureType(std::wstring value);
	
	bool IsSameAssociation(FeatureBinding& fb);
	void AppendFeatureBinding(FeatureBinding& fb);

	void SortFeatureType();
};