#include "stdafx.h"
#include "UnlimitedInteger.h"

#include <string>

UnlimitedInteger::UnlimitedInteger()
{

}

UnlimitedInteger::~UnlimitedInteger()
{

}

void UnlimitedInteger::GetContents(pugi::xml_node& node)
{
	std::wstring name = pugi::as_wide(node.name());

	if (auto attributeNode = node.attribute("infinite")) 
	{
		infinite = attributeNode.as_bool();

		if (infinite == false)
		{
			value = node.text().as_int();
		}
	}
}

int UnlimitedInteger::GetIntegerValue()
{
	return value;
}

bool UnlimitedInteger::IsInfinite()
{
	return infinite;
}

bool UnlimitedInteger::operator==(UnlimitedInteger& item)
{
	if (infinite == item.infinite &&
		value == item.value)
	{
		return true;
	}

	return false;
}