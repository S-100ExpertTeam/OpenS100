#include "stdafx.h"
#include "S100_Template.h"
#include "S100_NullInstruction.h"
#include "S100_PointInstruction.h"
#include "S100_LineInstruction.h"
#include "S100_AreaInstruction.h"
#include "S100_TextInstruction.h"

S100_Template::S100_Template()
{
	calltemplate = NULL;
	displayList = NULL;
}

S100_Template::~S100_Template()
{
	for (auto itor = instructions.begin(); itor != instructions.end(); itor++)
	{
		S100_Instruction *instruction = *itor;
		
		switch (instruction->GetType())
		{
		case 0:
			delete (S100_NullInstruction*)instruction;
			break;
		case 1: 
			delete (S100_PointInstruction*)instruction;
			break;
		case 2: 
			delete (S100_LineInstruction*)instruction;
			break;
		case 3: 
			delete (S100_AreaInstruction*)instruction;
			break;
		case 5: 
			delete (S100_TextInstruction*)instruction;
			break;
		}
	}
	if (calltemplate) delete calltemplate;
	if (displayList) delete displayList;
}

void S100_Template::SetMatch(std::wstring& value)
{
	match = value;
}
void S100_Template::SetPriority(std::wstring& value)
{
	priority = value;
}
void S100_Template::SetInstruction(S100_Instruction* value) 
{
	instructions.push_back(value);
}
void S100_Template::SetInstructions(std::vector<S100_Instruction*> value) 
{
	instructions = value;
}
void S100_Template::SetCallTemplate(S100_CallTemplate* value) 
{
	calltemplate = value;
}
void S100_Template::SetDisplayList(S100_DisplayList* value) 
{
	displayList = value;
}

std::wstring S100_Template::GetMatch() 
{
	return match;

}
std::wstring S100_Template::GetPriorty() 
{
	return priority;
}
S100_Instruction* S100_Template::GetInstruction(int index)
{
	return instructions.at(index);
}
std::vector<S100_Instruction*> S100_Template::GetInstruction() 
{
	return instructions;
}
S100_CallTemplate* S100_Template::GetCallTemplate()
{
	return calltemplate;
}
S100_DisplayList* S100_Template::GetDisplayList()
{
	return displayList;
}