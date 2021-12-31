#pragma once
#include "SENC_Instruction.h"

class SENC_AlertReference :
	public SENC_Instruction
{
public:
	SENC_AlertReference();
	virtual ~SENC_AlertReference();

public:
	/*
	0: none
	1: Prohibited Area
	2: Navigational Hazard
	3: Safety Contour
	*/
	int alertType = 0;
	std::wstring plan;
	std::wstring monitor;
};