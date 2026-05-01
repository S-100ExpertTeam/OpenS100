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
		const char* instructionName = instruction.name();
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
			std::string content = instructionName;
			content.append("is another data");
		}

	}
}

void Address::SetDeliveryPoint(char* value)
{
	deliveryPoint =value;
}

const std::string& Address::GetDeliveryPoint()
{
	return deliveryPoint;
}

void Address::SetCity(char* value) 
{
	city = value;
}

const std::string& Address::GetCity()
{
	return city;
}

void Address::SetAdministrativeArea(char* value) 
{
	administrativeArea = value;
}

const std::string& Address::GetAdministrativeArea()
{
	return administrativeArea;
}

void Address::SetPostalCode(char* value) 
{
	postalCode = value;
}

const std::string& Address::GetPostalCode()
{
	return postalCode;
}

void Address::SetCountry(char* value) 
{
	country = value;
}

const std::string& Address::GetCountry()
{
	return country;
}

void Address::SetElectronicMailAddress(char* value) 
{
	electronicMailAddress = value;
}

const std::string& Address::GetElectronicMailAddress()
{
	return electronicMailAddress;
}