#include "stdafx.h"
#include "Contact.h"

Contact::Contact(void)
{

}

Contact::~Contact(void)
{

}

void Contact::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "S100CI:phone"))
		{
			Telephone* phoneData = new Telephone();
			phoneData->GetContents(instruction);
			SetPhone(*phoneData);
		}
		else if (!strcmp(instructionName, "S100CI:address"))
		{
			Address* add = new Address();
			add->GetContents(instruction);
			SetAddress(*add);
		}
		else if (!strcmp(instructionName, "S100CI:onlineResource"))
		{
			OnlineResource* online = new OnlineResource();
			online->GetContents(instruction);
			SetOnlineResource(*online);
		}
		else if (!strcmp(instructionName, "S100CI:hoursOfService"))
		{
			SetHoursOfService((char*)instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:contactInstructions"))
		{
			SetContactInstructions((char*)instruction.child_value());
		}
		else 
		{
			std::wstring content = pugi::as_wide(instructionName);
			content.append(L" is another data");
		}
	}
}

void Contact::SetPhone(Telephone& value)
{
	phone = value;
}

const Telephone& Contact::Getphone()
{
	return phone;
}

void Contact::SetAddress(Address& value)
{
	address = value;
}

const Address& Contact::GetAddress()
{
	return address;
}

void Contact::SetOnlineResource(OnlineResource& value)
{
	onlineResource = value;
}

const OnlineResource& Contact::GetOnlineResource()
{
	return onlineResource;
}

void Contact::SetHoursOfService(char* value) 
{
	hoursOfService = pugi::as_wide(value);
}

const std::wstring& Contact::GetHoursOfService()
{
	return hoursOfService;
}

void Contact::SetContactInstructions(char* value) 
{
	contactInstructions = pugi::as_wide(value);
}

const std::wstring& Contact::GetContactInstructions()
{
	return contactInstructions;
}