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

void OnlineResource::SetUrl(std::string value)
{
	url = value;
}

const std::string& OnlineResource::GetUrl()
{
	return url;
}

void OnlineResource::SetProtocol(std::string value) 
{
	protocol = value;
}

const std::string& OnlineResource::GetProtocol()
{
	return protocol;
}

void OnlineResource::SetApplicationProfile(std::string value) 
{
	applicationProfile = value;
}
const std::string& OnlineResource::GetApplicationProfile()
{
	return applicationProfile;
}

void OnlineResource::SetName(std::string value) 
{
	name = value;
}
const std::string& OnlineResource::GetName()
{
	return name;
}

void OnlineResource::SetDescription(std::string value) 
{
	description = value;
}
const std::string& OnlineResource::GetDescription()
{
	return description;
}

void OnlineResource::SetFunction(OnlineFunctionCode value)
{
	function = value;
}

OnlineFunctionCode& OnlineResource::GetFunction() 
{
	return function;
}

void OnlineResource::SetLinkage(std::string value) 
{
	linkage = value;
}
const std::string& OnlineResource::GetLinkage()
{
	return linkage;
}

void OnlineResource::SetUrl(const char* value)
{
	std::string s(value ? value : "");
	SetUrl(s);
}

void OnlineResource::SetUrl(std::wstring value)
{
	std::string s = toUtf8(value);
	SetUrl(s);
}

std::wstring OnlineResource::GetUrlW()
{
	return toWide(GetUrl());
}

void OnlineResource::SetProtocol(const char* value)
{
	std::string s(value ? value : "");
	SetProtocol(s);
}

void OnlineResource::SetProtocol(std::wstring value)
{
	std::string s = toUtf8(value);
	SetProtocol(s);
}

std::wstring OnlineResource::GetProtocolW()
{
	return toWide(GetProtocol());
}

void OnlineResource::SetApplicationProfile(const char* value)
{
	std::string s(value ? value : "");
	SetApplicationProfile(s);
}

void OnlineResource::SetApplicationProfile(std::wstring value)
{
	std::string s = toUtf8(value);
	SetApplicationProfile(s);
}

std::wstring OnlineResource::GetApplicationProfileW()
{
	return toWide(GetApplicationProfile());
}

void OnlineResource::SetName(const char* value)
{
	std::string s(value ? value : "");
	SetName(s);
}

void OnlineResource::SetName(std::wstring value)
{
	std::string s = toUtf8(value);
	SetName(s);
}

std::wstring OnlineResource::GetNameW()
{
	return toWide(GetName());
}

void OnlineResource::SetDescription(const char* value)
{
	std::string s(value ? value : "");
	SetDescription(s);
}

void OnlineResource::SetDescription(std::wstring value)
{
	std::string s = toUtf8(value);
	SetDescription(s);
}

std::wstring OnlineResource::GetDescriptionW()
{
	return toWide(GetDescription());
}

void OnlineResource::SetLinkage(const char* value)
{
	std::string s(value ? value : "");
	SetLinkage(s);
}

void OnlineResource::SetLinkage(std::wstring value)
{
	std::string s = toUtf8(value);
	SetLinkage(s);
}

std::wstring OnlineResource::GetLinkageW()
{
	return toWide(GetLinkage());
}
