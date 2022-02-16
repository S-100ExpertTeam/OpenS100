#pragma once

#include "..\\extlibs\\pugixml\\include\pugixml.hpp"

#include <list>

class S100_Instruction;
class S100_AlertReference;

class S100_DisplayList
{
public:
	S100_DisplayList();
	virtual ~S100_DisplayList();

private:
	std::list<S100_Instruction*> displayInstructions;
	std::list<S100_Instruction*> alertInstructions;

public:
	void GetContents(pugi::xml_node node);
	void RemoveInstructions();

	void SetDisplayInstruction(S100_Instruction* value);
	void SetDisplayInstructions(std::list<S100_Instruction*> value);

	S100_Instruction* GetDisplayInstruction(int index);
	std::list<S100_Instruction*> GetDisplayInstructions();

	std::list<S100_Instruction*> GetAlertInstructions();
	void AddAlertInstruction(S100_Instruction* _ins);
};