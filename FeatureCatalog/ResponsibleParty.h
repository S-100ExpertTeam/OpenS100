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
	void SetIndividualName(std::wstring value) { SetIndividualName(toUtf8(value)); }
	const std::string& GetIndividualName();
	std::wstring GetIndividualNameW() { return toWide(GetIndividualName()); }

	void SetOrganisationName(std::string value);
	void SetOrganisationName(std::wstring value) { SetOrganisationName(toUtf8(value)); }
	const std::string& GetOrganisationName();
	std::wstring GetOrganisationNameW() { return toWide(GetOrganisationName()); }

	void SetPositionName(std::string value);
	void SetPositionName(std::wstring value) { SetPositionName(toUtf8(value)); }
	const std::string& GetPositionName();
	std::wstring GetPositionNameW() { return toWide(GetPositionName()); }

	void SetContactInfo(Contact value);
	Contact &GetContactInfo();

	void SetRoleCode(RoleCode value);
	RoleCode &GetRoleCode();

	void SetParty(Party value);
	Party &GetParty();
};
