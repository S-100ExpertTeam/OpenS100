#pragma once
#include "XML_Item.h"

#include <pugixml.hpp>

#include <string>
#include "StringUtil.h"

class Address :
	public XML_Item
{
public:
	Address();
	virtual ~Address();

private:
	std::string deliveryPoint = "";
	std::string city = "";
	std::string administrativeArea = "";
	std::string postalCode = "";
	std::string country = "";
	std::string electronicMailAddress = "";

public:
	void GetContents(pugi::xml_node& node);

	void SetDeliveryPoint(char* value);
	const std::string& GetDeliveryPoint();
	std::wstring GetDeliveryPointW();
	 
	void SetCity(char* value);
	const std::string& GetCity();
	std::wstring GetCityW();

	void SetAdministrativeArea(char* value);
	const std::string& GetAdministrativeArea();
	std::wstring GetAdministrativeAreaW();

	void SetPostalCode(char* value);
	const std::string& GetPostalCode();
	std::wstring GetPostalCodeW();

	void SetCountry(char* value);
	const std::string& GetCountry();
	std::wstring GetCountryW();

	void SetElectronicMailAddress(char* value);
	const std::string& GetElectronicMailAddress();
	std::wstring GetElectronicMailAddressW();
};
