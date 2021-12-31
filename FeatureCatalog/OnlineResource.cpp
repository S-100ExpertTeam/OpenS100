#include "stdafx.h"
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
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "S100CI:url"))
		{
			SetUrl(pugi::as_wide(instruction.child_value()));
		}
		else if (!strcmp(instructionName, "S100CI:protocol"))
		{
			SetProtocol(pugi::as_wide(instruction.child_value()));
		}
		else if (!strcmp(instructionName, "S100CI:applicationProfile"))
		{
			SetApplicationProfile(pugi::as_wide(instruction.child_value()));
		}
		else if (!strcmp(instructionName, "S100CI:name"))
		{
			SetName(pugi::as_wide(instruction.child_value()));
		}
		else if (!strcmp(instructionName, "S100CI:description"))
		{
			SetDescription(pugi::as_wide(instruction.child_value()));
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
			std::wstring content = pugi::as_wide(instructionName);
			content.append(L" is another data");
		}
	}
}

void OnlineResource::SetUrl(std::wstring value)
{
	url = value;
}

const std::wstring& OnlineResource::GetUrl()
{
	return url;
}

void OnlineResource::SetProtocol(std::wstring value) 
{
	protocol = value;
}

const std::wstring& OnlineResource::GetProtocol()
{
	return protocol;
}

void OnlineResource::SetApplicationProfile(std::wstring value) 
{
	applicationProfile = value;
}
const std::wstring& OnlineResource::GetApplicationProfile()
{
	return applicationProfile;
}

void OnlineResource::SetName(std::wstring value) 
{
	name = value;
}
const std::wstring& OnlineResource::GetName()
{
	return name;
}

void OnlineResource::SetDescription(std::wstring value) 
{
	description = value;
}
const std::wstring& OnlineResource::GetDescription()
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