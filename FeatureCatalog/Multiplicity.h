#pragma once
#include "XML_Item.h"
#include "UnlimitedInteger.h"

#include <pugixml.hpp>

class Multiplicity
{
public:
	Multiplicity();
	virtual ~Multiplicity();

private:
	int lower = 0;
	UnlimitedInteger upper;

public:
	void GetContents(pugi::xml_node& node);

	void SetLower(int value);
	const int GetLower();

	void SetUpper(UnlimitedInteger& value);
	UnlimitedInteger& GetUpper();

	bool IsInfinite();
	int GetUpperCount();

	bool operator==(Multiplicity& item);
};