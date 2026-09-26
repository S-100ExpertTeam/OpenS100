#pragma once
#include "XML_Item.h"
#include "Telephone.h"
#include "Address.h"
#include "OnlineResource.h"

#include <pugixml.hpp>
#include "StringUtil.h"

class Contact :
	public XML_Item
{
public:
	Contact(void);
	virtual ~Contact(void);
	
private:
	Telephone phone;
	Address address;
	OnlineResource onlineResource;
	std::string hoursOfService = "";
	std::string contactInstructions = "";
	
public:
	void GetContents(pugi::xml_node& node);

	void SetPhone(Telephone& value);
	const Telephone& Getphone();

	void SetAddress(Address& value);
	const Address& GetAddress();

	void SetOnlineResource(OnlineResource& value);
	const OnlineResource& GetOnlineResource();

	void SetHoursOfService(char* value);
	const std::string& GetHoursOfService();
	std::wstring GetHoursOfServiceW();

	void SetContactInstructions(char* value);
	const std::string& GetContactInstructions();
	std::wstring GetContactInstructionsW();
};
