#pragma once
#include <string>

// AbstractLineStyle
class S100_LineStyleBase
{
public:
	S100_LineStyleBase();
	virtual ~S100_LineStyleBase();
	
private:
	/*
	1 : Line Style
	2 : Line Style Reference
	3 : Complex Line Style
	*/
	int type;

public:
	void SetType(int value);
	int GetType();
};