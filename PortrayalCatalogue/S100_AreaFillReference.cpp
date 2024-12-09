#include "stdafx.h"
#include "S100_AreaFillReference.h"

S100_AreaFillReference::S100_AreaFillReference()
{
	SetType(2);
}

S100_AreaFillReference::~S100_AreaFillReference()
{

}

void S100_AreaFillReference::GetContents(pugi::xml_node node) 
{
	if (node == nullptr) 
	{
		return;
	}

	for (auto attri=node.first_attribute(); attri; attri=attri.next_attribute())
	{
		auto attriName = attri.name();
		if (!strcmp(attriName,"reference"))
		{
			reference = pugi::as_wide(attri.value());
			break;
		}
	}

	for (auto instruction=node.first_child(); instruction; instruction=instruction.next_sibling())
	{
		auto instructionName = node.name();
		if (!strcmp(instructionName,"reference"))
		{
			reference = pugi::as_wide(instruction.value());
			break;
		}
	}
}

void S100_AreaFillReference::SetReference(std::wstring& value)
{
	reference = value;
}

std::wstring S100_AreaFillReference::GetReference() 
{
	return reference;
}