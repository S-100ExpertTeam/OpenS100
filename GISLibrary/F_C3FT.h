#pragma once
#include "Field.h"

class F_C3FT :
	public Field
{
public:
	F_C3FT();
	virtual ~F_C3FT();

public:
	/*
	* Vertical CRS Id
	* Format		: b11
	* Description	: Internal identifier of the Vertical CRS
	*/
	int m_vcid;
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
	/*
	* Coordinate in Z axis
	* Format		: b48
	* Description	: Z-coordinate (depth or height)
	*/
	double m_zcoo;

public:
	static int GetSize();
	void ReadField(BYTE *&buf);
	BOOL Save(CFile *file);
	int GetFieldLength();
};