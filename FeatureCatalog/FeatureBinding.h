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
	std::wstring GetAssociationW() { return toWide(GetAssociation()); }
	std::string GetAssociationAsWstring();
	std::wstring GetAssociationAsWstringW() { return toWide(GetAssociationAsWstring()); }

	void SetAssociation(std::string value);
	void SetAssociation(std::wstring value) { SetAssociation(toUtf8(value)); }

	std::string GetRole();
	std::wstring GetRoleW() { return toWide(GetRole()); }
	std::string GetRoleAsWstring();
	std::wstring GetRoleAsWstringW() { return toWide(GetRoleAsWstring()); }

	void SetRole(std::string value);
	void SetRole(std::wstring value) { SetRole(toUtf8(value)); }

	S100_FC_RoleType GetRoleType();
	std::string GetRoleTypeAsString();
	std::string GetRoleTypeAsWstring();
	std::wstring GetRoleTypeAsWstringW() { return toWide(GetRoleTypeAsWstring()); }

	void SetRoleType(S100_FC_RoleType value);
	void SetRoleType(std::string value);
	void SetRoleType(std::wstring value) { SetRoleType(toUtf8(value)); }

	int GetFeatureTypeCount();

	std::string GetFeatureType(int index);
	std::vector<std::string> GetFeatureTypes();
	std::string GetFeatureTypeAsWstring(int index);

	void InsertFeatureType(std::string value);
	
	bool IsSameAssociation(FeatureBinding& fb);
	void AppendFeatureBinding(FeatureBinding& fb);

	void SortFeatureType();
};
