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