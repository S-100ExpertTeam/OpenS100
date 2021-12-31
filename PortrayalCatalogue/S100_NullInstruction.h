#pragma once
#include "S100_Instruction.h"

class S100_NullInstruction : public S100_Instruction
{
public:
	S100_NullInstruction();
	virtual ~S100_NullInstruction();

public:
	void GetContents(MSXML2::IXMLDOMNodePtr pNode);
	void GetContents(pugi::xml_node node);
};