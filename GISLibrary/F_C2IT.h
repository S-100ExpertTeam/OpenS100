#pragma once
#include "Field.h"

class F_C2IT :
	public Field
{
public:
	F_C2IT();
	virtual ~F_C2IT();

public:
	/*
	* Coordinate in Y axis
	* Format		: b24
	* Description	: Y-coordinate or latitude
	*/
	int m_ycoo;
	/*
	* Coordinate in X axis
	* Format		: b24
	* Description	: X-coordinate or longitude
	*/
	int m_xcoo;
	static int GetSize();

public:
	void ReadField(BYTE *&buf);
	int GetFieldLength();
};