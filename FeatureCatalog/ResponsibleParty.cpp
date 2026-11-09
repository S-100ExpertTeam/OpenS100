#include "stdafx.h"
#include "StringUtil.h"
#include "ResponsibleParty.h"

ResponsibleParty::ResponsibleParty()
{

}

ResponsibleParty::~ResponsibleParty()
{

}

void ResponsibleParty::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const char* instructionName = instruction.name();

		if (!strcmp(instructionName, "S100CI:individualName"))
		{
			individualName = instruction.child_value();
		}
		else if (!strcmp(instructionName, "S100CI:organisationName"))
		{
			organisationName = instruction.child_value();
		}
		else if (!strcmp(instructionName, "S100CI:positionName"))
		{
			positionName = instruction.child_value();
		}
		else if (!strcmp(instructionName, "S100CI:contactInfo"))
		{
			contactInfo.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100CI:role"))
		{
			role.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100CI:party"))
		{
			party.GetContents(instruction);

		}
		else
		{
			std::string content = instruction.child_value();
			content.append(" is another data");
		}
	}
}

void ResponsibleParty::SetIndividualName(std::string _value)
{
	individualName = _value;
}

const std::string& ResponsibleParty::GetIndividualName()
{
	return individualName;
}

void ResponsibleParty::SetOrganisationName(std::string _value)
{
	organisationName = _value;
}
const std::string& ResponsibleParty::GetOrganisationName()
{
	return organisationName;
}

void ResponsibleParty::SetPositionName(std::string _value)
{
	positionName = _value;
}

const std::string& ResponsibleParty::GetPositionName()
{
	return positionName;
}

void ResponsibleParty::SetContactInfo(Contact _value)
{
	contactInfo = _value;
}

Contact &ResponsibleParty::GetContactInfo()
{
	return contactInfo;
}

void ResponsibleParty::SetRoleCode(RoleCode _value)
{
	role = _value;
}
RoleCode &ResponsibleParty::GetRoleCode()
{
	return role;
}

void ResponsibleParty::SetParty(Party _value)
{
	party = _value;
}

Party &ResponsibleParty::GetParty()
{
	return party;
}

void ResponsibleParty::SetIndividualName(const char* _value)
{
	std::string s(_value ? _value : "");
	SetIndividualName(s);
}

void ResponsibleParty::SetIndividualName(std::wstring _value)
{
	std::string s = toUtf8(_value);
	SetIndividualName(s);
}

std::wstring ResponsibleParty::GetIndividualNameW()
{
	return toWide(GetIndividualName());
}

void ResponsibleParty::SetOrganisationName(const char* _value)
{
	std::string s(_value ? _value : "");
	SetOrganisationName(s);
}

void ResponsibleParty::SetOrganisationName(std::wstring _value)
{
	std::string s = toUtf8(_value);
	SetOrganisationName(s);
}

std::wstring ResponsibleParty::GetOrganisationNameW()
{
	return toWide(GetOrganisationName());
}

void ResponsibleParty::SetPositionName(const char* _value)
{
	std::string s(_value ? _value : "");
	SetPositionName(s);
}

void ResponsibleParty::SetPositionName(std::wstring _value)
{
	std::string s = toUtf8(_value);
	SetPositionName(s);
}

std::wstring ResponsibleParty::GetPositionNameW()
{
	return toWide(GetPositionName());
}
