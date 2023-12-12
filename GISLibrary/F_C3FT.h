#pragma once
#include "Field.h"

class F_C3FT :
	public Field
{
public:
	F_C3FT();
	F_C3FT(const F_C3FT& other);
	virtual ~F_C3FT();

public:
	/*
	* Vertical CRS Id
	* Format		: b11
	* Description	: Internal identifier of the Vertical CRS
	*/
	int m_vcid = 2;
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
	int GetFieldLength();
};