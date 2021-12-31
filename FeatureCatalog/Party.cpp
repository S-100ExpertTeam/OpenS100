#include "stdafx.h"
#include "Party.h"

Party::Party()
{

}

Party::~Party()
{

}

void Party::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling()) 
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName,"S100CI:CI_Organisation"))
		{
			ci_Organisation.GetContents(instruction);
		}
	}
}

void Party::SetCi_Organisation(CI_Organisation value) 
{
	ci_Organisation = value;
}

CI_Organisation& Party::GetCi_Organisation()
{
	return ci_Organisation;
}