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
	void SetUrl(std::wstring value) { SetUrl(toUtf8(value)); }
	const std::string& GetUrl();
	std::wstring GetUrlW() { return toWide(GetUrl()); }

	void SetProtocol(std::string value);
	void SetProtocol(std::wstring value) { SetProtocol(toUtf8(value)); }
	const std::string& GetProtocol();
	std::wstring GetProtocolW() { return toWide(GetProtocol()); }

	void SetApplicationProfile(std::string value);
	void SetApplicationProfile(std::wstring value) { SetApplicationProfile(toUtf8(value)); }
	const std::string& GetApplicationProfile();
	std::wstring GetApplicationProfileW() { return toWide(GetApplicationProfile()); }

	void SetName(std::string value);
	void SetName(std::wstring value) { SetName(toUtf8(value)); }
	const std::string& GetName();
	std::wstring GetNameW() { return toWide(GetName()); }

	void SetDescription(std::string value);
	void SetDescription(std::wstring value) { SetDescription(toUtf8(value)); }
	const std::string& GetDescription();
	std::wstring GetDescriptionW() { return toWide(GetDescription()); }

	void SetFunction(OnlineFunctionCode value);
	OnlineFunctionCode& GetFunction();

	void SetLinkage(std::string value);
	void SetLinkage(std::wstring value) { SetLinkage(toUtf8(value)); }
	const std::string& GetLinkage();
	std::wstring GetLinkageW() { return toWide(GetLinkage()); }
};
