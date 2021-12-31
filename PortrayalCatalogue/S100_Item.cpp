#include "stdafx.h"
#include "S100_Item.h"

S100_Item::S100_Item()
{

}

S100_Item::~S100_Item()
{

}

void S100_Item::GetContents(pugi::xml_node Node)
{
	auto tokenNode = Node.first_attribute();
	if (!strcmp(tokenNode.name(), "token"))
	{
		token = pugi::as_wide(tokenNode.value());
	}

	for (pugi::xml_node instruction = Node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName, "cie"))
		{
			scie.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "srgb"))
		{
			srgb.GetContents(instruction);
		}
	}
}

void S100_Item::SetToken(std::wstring& value)
{
	token = value;
}

void S100_Item::SetSCIE(S100_CIE* value)
{
	scie = *value;
}

void S100_Item::SetSRGB(S100_SRGB* value)
{
	srgb = *value;
}

std::wstring S100_Item::GetToken()
{
	return token;
}

S100_CIE* S100_Item::GetSCIE()
{
	return &scie;
}

S100_SRGB* S100_Item::GetSRGB()
{
	return &srgb;
}