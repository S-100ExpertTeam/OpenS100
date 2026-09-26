#include "stdafx.h"
#include "Multiplicity.h"

#include "..\\LatLonUtility\\cpp_util.h"

#include <string>

Multiplicity::Multiplicity()
{

}

Multiplicity::~Multiplicity()
{

}

void Multiplicity::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const char* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100Base:lower"))
		{
			lower = cpp_util::stoi(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100Base:upper"))
		{
			upper.GetContents(instruction);
		}
	}
}
void Multiplicity::SetLower(int value)
{
	lower = value;
}

const int Multiplicity::GetLower() 
{
	return lower;
}

void Multiplicity::SetUpper(UnlimitedInteger& value) 
{
	upper = value;
}

UnlimitedInteger& Multiplicity::GetUpper()
{
	return upper;
}

bool Multiplicity::IsInfinite()
{
	return upper.IsInfinite();
}

int Multiplicity::GetUpperCount()
{
	return upper.GetIntegerValue();
}

bool Multiplicity::operator==(Multiplicity& item)
{
	if (lower == item.GetLower() &&
		upper == item.GetUpper())
	{
		return true;
	}

	return false;
}