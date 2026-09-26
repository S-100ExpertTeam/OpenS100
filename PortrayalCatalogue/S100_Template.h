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
	std::string match;
	std::string priority;
	std::vector<S100_Instruction*>instructions;
	S100_CallTemplate* calltemplate;
	S100_DisplayList *displayList;

public:
	void SetMatch(const std::string& value);
	void SetMatch(const std::wstring& value);
	void SetPriority(const std::string& value);
	void SetPriority(const std::wstring& value);
	void SetInstruction(S100_Instruction* value);
	void SetInstructions(std::vector<S100_Instruction*> value);
	void SetCallTemplate(S100_CallTemplate* value);
	void SetDisplayList(S100_DisplayList* value);

	std::string  GetMatch();
	std::wstring GetMatchW();
	std::string  GetPriorty();
	std::wstring GetPriortyW();
	S100_Instruction* GetInstruction(int index);
	std::vector<S100_Instruction*> GetInstruction();
	S100_CallTemplate* GetCallTemplate();
	S100_DisplayList* GetDisplayList();
};
