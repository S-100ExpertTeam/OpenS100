#pragma once
#include "XML_Item.h"
#include "OnlineFunctionCode.h"

#include <pugixml.hpp>

#include <string>

class OnlineResource :
	public XML_Item
{
public:
	OnlineResource();
	virtual ~OnlineResource();

private:
	std::string url;
	std::string protocol;
	std::string applicationProfile;
	std::string name;
	std::string description;
	OnlineFunctionCode function; 
	std::string linkage;

public:
	void GetContents(pugi::xml_node& node);

	void SetUrl(std::string value);
	const std::string& GetUrl();

	void SetProtocol(std::string value);
	const std::string& GetProtocol();

	void SetApplicationProfile(std::string value);
	const std::string& GetApplicationProfile();

	void SetName(std::string value);
	const std::string& GetName();

	void SetDescription(std::string value);
	const std::string& GetDescription();

	void SetFunction(OnlineFunctionCode value);
	OnlineFunctionCode& GetFunction();

	void SetLinkage(std::string value);
	const std::string& GetLinkage();
};