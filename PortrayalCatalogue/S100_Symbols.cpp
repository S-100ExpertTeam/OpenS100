#include "stdafx.h"
#include "S100_Symbols.h"
#include "S100_Symbol.h"

S100_Symbols::S100_Symbols() {

}

S100_Symbols::~S100_Symbols() 
{
	for (auto i = s100_symbols_v.begin(); i != s100_symbols_v.end(); i++)
	{
		delete *i;
	}
}

void S100_Symbols::GetContents(pugi::xml_node& node) 
{
	for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling()) 
	{
		auto instructionName = instruction.name();

		if (!strcmp(instructionName,"symbol")) 
		{
			S100_Symbol* symbol = new S100_Symbol();
			symbol->GetContents(instruction);
	
			s100_symbols_v.push_back(symbol);
			s100_symbols.insert({ symbol->GetId(),symbol });
		}
	}
}