#pragma once

#include <string>

enum class AttributeVisibility
{
	none = 0,
	publicVisibility = 1,
	protectedVisibility = 2,
	privateVisibility = 3
};

AttributeVisibility stringToAttributeVisibility(std::string value);
std::string attributeVisibilityToString(AttributeVisibility value);