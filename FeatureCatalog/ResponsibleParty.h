#pragma once
#include "XML_Item.h"
#include "Contact.h"
#include "RoleCode.h"
#include "Party.h"

#include <pugixml.hpp>

#include <string>
#include "StringUtil.h"

class ResponsibleParty :
	public XML_Item
{
public:
	ResponsibleParty();
	virtual ~ResponsibleParty();

private:
	std::string individualName = "";
	std::string organisationName = "";
	std::string positionName = "";
	Contact contactInfo; 
	RoleCode role;
	Party party;

public:
	void GetContents(pugi::xml_node& node);

	void SetIndividualName(std::string value);
	void SetIndividualName(const char* value);
	void SetIndividualName(std::wstring value);
	const std::string& GetIndividualName();
	std::wstring GetIndividualNameW();

	void SetOrganisationName(std::string value);
	void SetOrganisationName(const char* value);
	void SetOrganisationName(std::wstring value);
	const std::string& GetOrganisationName();
	std::wstring GetOrganisationNameW();

	void SetPositionName(std::string value);
	void SetPositionName(const char* value);
	void SetPositionName(std::wstring value);
	const std::string& GetPositionName();
	std::wstring GetPositionNameW();

	void SetContactInfo(Contact value);
	Contact &GetContactInfo();

	void SetRoleCode(RoleCode value);
	RoleCode &GetRoleCode();

	void SetParty(Party value);
	Party &GetParty();
};
