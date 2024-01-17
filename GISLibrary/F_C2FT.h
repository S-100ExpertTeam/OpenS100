#pragma once
#include "Field.h"
class F_C2FT :
	public Field
{
public:
	F_C2FT();
	F_C2FT(const F_C2FT& other);
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
	virtual F_C2FT operator=(const F_C2FT& other);

public:
	void ReadField(BYTE *&buf);
	bool WriteField(CFile* file) { return true; }
	int GetFieldLength();
};