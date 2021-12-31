#include "stdafx.h"
#include "Multiplicity.h"

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
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "S100Base:lower"))
		{
			lower = std::stoi(instruction.child_value());
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