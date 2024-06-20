#pragma once

#include <pugixml.hpp>

class UnlimitedInteger
{
public:
	UnlimitedInteger();
	virtual ~UnlimitedInteger();

	bool infinite = false;
	int value = 0;

public:
	void GetContents(pugi::xml_node& node);

	int GetIntegerValue();

	bool IsInfinite();

	bool operator==(UnlimitedInteger& item);
};