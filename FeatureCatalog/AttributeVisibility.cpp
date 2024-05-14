#include "stdafx.h"
#include "AttributeVisibility.h"

AttributeVisibility stringToAttributeVisibility(std::string value)
{
	if (value.compare("publicVisibility") == 0)
	{
		return AttributeVisibility::publicVisibility;
	}
	else if (value.compare("protectedVisibility") == 0)
	{
		return AttributeVisibility::protectedVisibility;
	}
	else if (value.compare("privateVisibility") == 0)
	{
		return AttributeVisibility::privateVisibility;
	}

	return AttributeVisibility::none;
}


std::string attributeVisibilityToString(AttributeVisibility value)
{
	if (value == AttributeVisibility::publicVisibility)
	{
		return "publicVisibility";
	}
	else if (value == AttributeVisibility::protectedVisibility)
	{
		return "protectedVisibility";
	}
	else if (value == AttributeVisibility::privateVisibility)
	{
		return "privateVisibility";
	}

	return "";
}