#pragma once
#include "SENC_InstructionVisitor.h"

#include <vector>

class PortrayalCatalogue;
class SCurve;
class Scaler;

// AbstractLineStyle
class SENC_LineStyleBase
{
public:
	SENC_LineStyleBase();
	virtual ~SENC_LineStyleBase();

public: 
	/*
	* 1 : Line Style
	* 2 : Line Style Reference
	* 3 : Composite Line Style
	*/
	int type = 0; 

public:
	void SetType(int value);
	int GetType();

	virtual void Accept(SENC_LineStyleVisitor& /*visitor*/) {}

	bool IsLineStyle();
	bool IsLineStyleReference();
	bool IsCompositeLineStyle();
};