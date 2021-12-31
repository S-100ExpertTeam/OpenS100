#pragma once
#include "XML_Item.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <string>

class Address :
	public XML_Item
{
public:
	Address();
	virtual ~Address();

private:
	std::wstring deliveryPoint = L"";
	std::wstring city = L"";
	std::wstring administrativeArea = L"";
	std::wstring postalCode = L"";
	std::wstring country = L"";
	std::wstring electronicMailAddress = L"";

public:
	void GetContents(pugi::xml_node& node);

	void SetDeliveryPoint(char* value);
	const std::wstring& GetDeliveryPoint();
	 
	void SetCity(char* value);
	const std::wstring& GetCity();

	void SetAdministrativeArea(char* value);
	const std::wstring& GetAdministrativeArea();

	void SetPostalCode(char* value);
	const std::wstring& GetPostalCode();

	void SetCountry(char* value);
	const std::wstring& GetCountry();

	void SetElectronicMailAddress(char* value);
	const std::wstring& GetElectronicMailAddress();
};