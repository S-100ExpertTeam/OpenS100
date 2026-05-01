#pragma once
#include "XML_Item.h"
#include "OnlineFunctionCode.h"

#include <pugixml.hpp>

#include <string>
#include "StringUtil.h"

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
	void SetUrl(const char* value);
	void SetUrl(std::wstring value);
	const std::string& GetUrl();
	std::wstring GetUrlW();

	void SetProtocol(std::string value);
	void SetProtocol(const char* value);
	void SetProtocol(std::wstring value);
	const std::string& GetProtocol();
	std::wstring GetProtocolW();

	void SetApplicationProfile(std::string value);
	void SetApplicationProfile(const char* value);
	void SetApplicationProfile(std::wstring value);
	const std::string& GetApplicationProfile();
	std::wstring GetApplicationProfileW();

	void SetName(std::string value);
	void SetName(const char* value);
	void SetName(std::wstring value);
	const std::string& GetName();
	std::wstring GetNameW();

	void SetDescription(std::string value);
	void SetDescription(const char* value);
	void SetDescription(std::wstring value);
	const std::string& GetDescription();
	std::wstring GetDescriptionW();

	void SetFunction(OnlineFunctionCode value);
	OnlineFunctionCode& GetFunction();

	void SetLinkage(std::string value);
	void SetLinkage(const char* value);
	void SetLinkage(std::wstring value);
	const std::string& GetLinkage();
	std::wstring GetLinkageW();
};
