#pragma once
#include "SENC_Instruction.h"
#include "SENC_Attribute.h"

#include <list>
#include <vector>

class SENC_PointInstruction;
class SENC_LineInstruction;
class SENC_AreaInstruction;
class SENC_TextInstruction;
class SENC_CallTemplate;
class SENC_Template
{
public:
	SENC_Template();
	virtual ~SENC_Template();

public:
	std::list<SENC_Instruction*> instructions;
	std::vector<SENC_PointInstruction*> pointInstructions;
	std::vector<SENC_LineInstruction*> curveInstructions;
	std::vector<SENC_AreaInstruction*> surfaceInstructions;
	std::vector<SENC_TextInstruction*> textInstructions;
	SENC_CallTemplate* callTemplate;

	/*
	* Geometry Type of Feature
	* 1 : Point
	* 2 : Curve 
	* 3 : Surface
	*/
	int type;
	unsigned priority;

	std::wstring camelCase;
	std::vector<SENC_Attribute*> matchAttr;
};

