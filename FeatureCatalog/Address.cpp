#include "stdafx.h"
#include "Address.h"

Address::Address()
{

}

Address::~Address()
{

}

void Address::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100CI:deliveryPoint"))
		{
			SetDeliveryPoint((char*)instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:city")) 
		{
			SetCity((char*)instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:administrativeArea"))
		{
			SetAdministrativeArea((char*)instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:postalCode"))
		{
			SetPostalCode((char*)instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:country"))
		{
			SetCountry((char*)instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:electronicMailAddress"))
		{
			SetElectronicMailAddress((char*)instruction.child_value());
		}
		else 
		{
			std::wstring content = pugi::as_wide(instructionName);
			content.append(L"is another data");
		}

	}
}

void Address::SetDeliveryPoint(char* value)
{
	deliveryPoint =pugi::as_wide(value);
}

const std::wstring& Address::GetDeliveryPoint()
{
	return deliveryPoint;
}

void Address::SetCity(char* value) 
{
	city = pugi::as_wide(value);
}

const std::wstring& Address::GetCity()
{
	return city;
}

void Address::SetAdministrativeArea(char* value) 
{
	administrativeArea = pugi::as_wide(value);
}

const std::wstring& Address::GetAdministrativeArea()
{
	return administrativeArea;
}

void Address::SetPostalCode(char* value) 
{
	postalCode = pugi::as_wide(value);
}

const std::wstring& Address::GetPostalCode()
{
	return postalCode;
}

void Address::SetCountry(char* value) 
{
	country = pugi::as_wide(value);
}

const std::wstring& Address::GetCountry()
{
	return country;
}

void Address::SetElectronicMailAddress(char* value) 
{
	electronicMailAddress = pugi::as_wide(value);
}

const std::wstring& Address::GetElectronicMailAddress()
{
	return electronicMailAddress;
}