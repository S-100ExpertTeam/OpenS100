#pragma once
#include "XML_Item.h"

#include <pugixml.hpp>

#include <string>

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
	 
	void SetCity(char* value);
	const std::string& GetCity();

	void SetAdministrativeArea(char* value);
	const std::string& GetAdministrativeArea();

	void SetPostalCode(char* value);
	const std::string& GetPostalCode();

	void SetCountry(char* value);
	const std::string& GetCountry();

	void SetElectronicMailAddress(char* value);
	const std::string& GetElectronicMailAddress();
};