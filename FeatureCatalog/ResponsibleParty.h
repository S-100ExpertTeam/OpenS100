#pragma once
#include "XML_Item.h"
#include "Contact.h"
#include "RoleCode.h"
#include "Party.h"

#include <pugixml.hpp>

#include <string>

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
	const std::string& GetIndividualName();

	void SetOrganisationName(std::string value);
	const std::string& GetOrganisationName();

	void SetPositionName(std::string value);
	const std::string& GetPositionName();

	void SetContactInfo(Contact value);
	Contact &GetContactInfo();

	void SetRoleCode(RoleCode value);
	RoleCode &GetRoleCode();

	void SetParty(Party value);
	Party &GetParty();
};