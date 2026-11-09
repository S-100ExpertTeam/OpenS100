#include "stdafx.h"
#include "StringUtil.h"
#include "OnlineResource.h"

OnlineResource::OnlineResource()
{

}

OnlineResource::~OnlineResource()
{

}

void OnlineResource::GetContents(pugi::xml_node& node)
{
	//S100CI
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const char* instructionName = instruction.name();

		if (!strcmp(instructionName, "S100CI:url"))
		{
			SetUrl(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:protocol"))
		{
			SetProtocol(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:applicationProfile"))
		{
			SetApplicationProfile(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:name"))
		{
			SetName(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:description"))
		{
			SetDescription(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100CI:function"))
		{
			function.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "S100CI:linkage"))
		{
			SetLinkage(instruction.child_value());
		}
		else
		{
			std::string content = instructionName;
			content.append(" is another data");
		}
	}
}

void OnlineResource::SetUrl(std::string _value)
{
	url = _value;
}

const std::string& OnlineResource::GetUrl()
{
	return url;
}

void OnlineResource::SetProtocol(std::string _value) 
{
	protocol = _value;
}

const std::string& OnlineResource::GetProtocol()
{
	return protocol;
}

void OnlineResource::SetApplicationProfile(std::string _value) 
{
	applicationProfile = _value;
}
const std::string& OnlineResource::GetApplicationProfile()
{
	return applicationProfile;
}

void OnlineResource::SetName(std::string _value) 
{
	name = _value;
}
const std::string& OnlineResource::GetName()
{
	return name;
}

void OnlineResource::SetDescription(std::string _value) 
{
	description = _value;
}
const std::string& OnlineResource::GetDescription()
{
	return description;
}

void OnlineResource::SetFunction(OnlineFunctionCode _value)
{
	function = _value;
}

OnlineFunctionCode& OnlineResource::GetFunction() 
{
	return function;
}

void OnlineResource::SetLinkage(std::string _value) 
{
	linkage = _value;
}
const std::string& OnlineResource::GetLinkage()
{
	return linkage;
}

void OnlineResource::SetUrl(const char* _value)
{
	std::string s(_value ? _value : "");
	SetUrl(s);
}

void OnlineResource::SetUrl(std::wstring _value)
{
	std::string s = toUtf8(_value);
	SetUrl(s);
}

std::wstring OnlineResource::GetUrlW()
{
	return toWide(GetUrl());
}

void OnlineResource::SetProtocol(const char* _value)
{
	std::string s(_value ? _value : "");
	SetProtocol(s);
}

void OnlineResource::SetProtocol(std::wstring _value)
{
	std::string s = toUtf8(_value);
	SetProtocol(s);
}

std::wstring OnlineResource::GetProtocolW()
{
	return toWide(GetProtocol());
}

void OnlineResource::SetApplicationProfile(const char* _value)
{
	std::string s(_value ? _value : "");
	SetApplicationProfile(s);
}

void OnlineResource::SetApplicationProfile(std::wstring _value)
{
	std::string s = toUtf8(_value);
	SetApplicationProfile(s);
}

std::wstring OnlineResource::GetApplicationProfileW()
{
	return toWide(GetApplicationProfile());
}

void OnlineResource::SetName(const char* _value)
{
	std::string s(_value ? _value : "");
	SetName(s);
}

void OnlineResource::SetName(std::wstring _value)
{
	std::string s = toUtf8(_value);
	SetName(s);
}

std::wstring OnlineResource::GetNameW()
{
	return toWide(GetName());
}

void OnlineResource::SetDescription(const char* _value)
{
	std::string s(_value ? _value : "");
	SetDescription(s);
}

void OnlineResource::SetDescription(std::wstring _value)
{
	std::string s = toUtf8(_value);
	SetDescription(s);
}

std::wstring OnlineResource::GetDescriptionW()
{
	return toWide(GetDescription());
}

void OnlineResource::SetLinkage(const char* _value)
{
	std::string s(_value ? _value : "");
	SetLinkage(s);
}

void OnlineResource::SetLinkage(std::wstring _value)
{
	std::string s = toUtf8(_value);
	SetLinkage(s);
}

std::wstring OnlineResource::GetLinkageW()
{
	return toWide(GetLinkage());
}
