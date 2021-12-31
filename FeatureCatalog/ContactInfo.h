#pragma once
#include "XML_Item.h"
#include "Address.h"
#include "OnlineResource.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

class ContactInfo :
	public XML_Item
{
public:
	ContactInfo();
	virtual ~ContactInfo();

private:
	Address address;
	OnlineResource onlineResource;

public:
	void GetContents(pugi::xml_node& node);

	void SetAddress(Address& value);
	Address& GetAddress();

	void SetOnlineResource(OnlineResource& value);
	OnlineResource& GetOnlineResource();
};