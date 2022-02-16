#include "stdafx.h"
#include "S100_FeatureReference.h"

S100_FeatureReference::S100_FeatureReference()
{

}

S100_FeatureReference::~S100_FeatureReference()
{

}

void S100_FeatureReference::GetContents(pugi::xml_node node)
{
	if (node == nullptr)
	{
		return;
	}

	for (auto attri = node.first_attribute(); attri; attri = attri.next_attribute())
	{
		auto attriName = attri.name();
		if (!strcmp(attriName, "reference"))
		{
			reference = pugi::as_wide(attri.value());
		}
		reference = pugi::as_wide(node.child_value());
	}

}

void S100_FeatureReference::SetReference(std::wstring& value)
{
	reference = value;
}

std::wstring S100_FeatureReference::GetReference()
{
	return reference;
}