#include "stdafx.h"
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
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "S100CI:individualName"))
		{
			individualName = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:organisationName"))
		{
			organisationName = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:positionName"))
		{
			positionName = pugi::as_wide(instruction.child_value());
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
			std::wstring content = pugi::as_wide(instruction.child_value());
			content.append(L" is another data");
		}
	}
}

void ResponsibleParty::SetIndividualName(std::wstring value)
{
	individualName = value;
}

const std::wstring& ResponsibleParty::GetIndividualName()
{
	return individualName;
}

void ResponsibleParty::SetOrganisationName(std::wstring value)
{
	organisationName = value;
}
const std::wstring& ResponsibleParty::GetOrganisationName()
{
	return organisationName;
}

void ResponsibleParty::SetPositionName(std::wstring value)
{
	positionName = value;
}

const std::wstring& ResponsibleParty::GetPositionName()
{
	return positionName;
}

void ResponsibleParty::SetContactInfo(Contact value)
{
	contactInfo = value;
}

Contact &ResponsibleParty::GetContactInfo()
{
	return contactInfo;
}

void ResponsibleParty::SetRoleCode(RoleCode value)
{
	role = value;
}
RoleCode &ResponsibleParty::GetRoleCode()
{
	return role;
}

void ResponsibleParty::SetParty(Party value)
{
	party = value;
}

Party &ResponsibleParty::GetParty()
{
	return party;
}