#pragma once

#include "Multiplicity.h"
#include "Reference.h"
#include "S100_FC_RoleType.h"

#include <pugixml.hpp>
#include <vector>
#include "StringUtil.h"

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
	std::wstring GetAssociationW();
	std::string GetAssociationAsWstring();
	std::wstring GetAssociationAsWstringW();

	void SetAssociation(std::string value);
	void SetAssociation(const char* value);
	void SetAssociation(std::wstring value);

	std::string GetRole();
	std::wstring GetRoleW();
	std::string GetRoleAsWstring();
	std::wstring GetRoleAsWstringW();

	void SetRole(std::string value);
	void SetRole(const char* value);
	void SetRole(std::wstring value);

	S100_FC_RoleType GetRoleType();
	std::string GetRoleTypeAsString();
	std::string GetRoleTypeAsWstring();
	std::wstring GetRoleTypeAsWstringW();

	void SetRoleType(S100_FC_RoleType value);
	void SetRoleType(std::string value);
	void SetRoleType(const char* value);
	void SetRoleType(std::wstring value);

	int GetFeatureTypeCount();

	std::string GetFeatureType(int index);
	std::vector<std::string> GetFeatureTypes();
	std::string GetFeatureTypeAsWstring(int index);

	void InsertFeatureType(std::string value);
	
	bool IsSameAssociation(FeatureBinding& fb);
	void AppendFeatureBinding(FeatureBinding& fb);

	void SortFeatureType();
};
