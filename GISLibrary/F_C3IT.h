#pragma once
#include "Field.h"

class F_C3IT :
	public Field
{
public:
	F_C3IT();
	virtual ~F_C3IT();

public:
	/*
	* Vertical CRS Id
	* Format		: b11
	* Description	: Internal identifier of the Vertical CRS
	*/
	int m_vcid = 2;
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
	/*
	* Coordinate in Z axis
	* Format		: b24
	* Description	: Z-coordinate (depth or height)
	*/
	int m_zcoo;
	static int GetSize();

public:
	void ReadField(BYTE *&buf);
	int GetFieldLength();
};