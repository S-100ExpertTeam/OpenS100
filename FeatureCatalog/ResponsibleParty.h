#pragma once
#include "XML_Item.h"
#include "Contact.h"
#include "RoleCode.h"
#include "Party.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <string>

class ResponsibleParty :
	public XML_Item
{
public:
	ResponsibleParty();
	virtual ~ResponsibleParty();

private:
	std::wstring individualName = L"";
	std::wstring organisationName = L"";
	std::wstring positionName = L"";
	Contact contactInfo; 
	RoleCode role;
	Party party;

public:
	void GetContents(pugi::xml_node& node);

	void SetIndividualName(std::wstring value);
	const std::wstring& GetIndividualName();

	void SetOrganisationName(std::wstring value);
	const std::wstring& GetOrganisationName();

	void SetPositionName(std::wstring value);
	const std::wstring& GetPositionName();

	void SetContactInfo(Contact value);
	Contact &GetContactInfo();

	void SetRoleCode(RoleCode value);
	RoleCode &GetRoleCode();

	void SetParty(Party value);
	Party &GetParty();
};