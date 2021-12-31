#pragma once
#include "Field.h"
class F_C2FT :
	public Field
{
public:
	F_C2FT();
	virtual ~F_C2FT();
public:
	/*
	* Coordinate in Y axis
	* Format		: b48
	* Description	: Y-coordinate or latitude
	*/
	double m_ycoo;
	/*
	* Coordinate in X axis
	* Format		: b48
	* Description	: X-coordinate or longitude
	*/
	double m_xcoo;
	static int GetSize();

public:
	void ReadField(BYTE *&buf);
	BOOL Save(CFile *file);
	int GetFieldLength();
};