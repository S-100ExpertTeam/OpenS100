#pragma once

#include "S100_Instruction.h"
#include "S100_DisplayList.h"
#include "S100_CallTemplate.h"

#include <vector>

class S100_Template
{
public:
	S100_Template();
	virtual ~S100_Template();

private:
	std::wstring match;
	std::wstring priority;
	std::vector<S100_Instruction*>instructions;
	S100_CallTemplate* calltemplate;
	S100_DisplayList *displayList;

public:
	void SetMatch(std::wstring& value);
	void SetPriority(std::wstring& value);
	void SetInstruction(S100_Instruction* value);
	void SetInstructions(std::vector<S100_Instruction*> value);
	void SetCallTemplate(S100_CallTemplate* value);
	void SetDisplayList(S100_DisplayList* value);

	std::wstring GetMatch();
	std::wstring GetPriorty();
	S100_Instruction* GetInstruction(int index);
	std::vector<S100_Instruction*> GetInstruction();
	S100_CallTemplate* GetCallTemplate();
	S100_DisplayList* GetDisplayList();
};