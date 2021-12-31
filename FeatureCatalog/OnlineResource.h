#pragma once
#include "XML_Item.h"
#include "OnlineFunctionCode.h"

#include "..\\extlibs\\pugixml\\include\\pugixml.hpp"

#include <string>

class OnlineResource :
	public XML_Item
{
public:
	OnlineResource();
	virtual ~OnlineResource();

private:
	std::wstring url = L"";
	std::wstring protocol = L"";
	std::wstring applicationProfile = L"";
	std::wstring name = L"";
	std::wstring description = L"";
	OnlineFunctionCode function; 
	std::string linkage = "";

public:
	void GetContents(pugi::xml_node& node);

	void SetUrl(std::wstring value);
	const std::wstring& GetUrl();

	void SetProtocol(std::wstring value);
	const std::wstring& GetProtocol();

	void SetApplicationProfile(std::wstring value);
	const std::wstring& GetApplicationProfile();

	void SetName(std::wstring value);
	const std::wstring& GetName();

	void SetDescription(std::wstring value);
	const std::wstring& GetDescription();

	void SetFunction(OnlineFunctionCode value);
	OnlineFunctionCode& GetFunction();

	void SetLinkage(std::string value);
	const std::string& GetLinkage();
};