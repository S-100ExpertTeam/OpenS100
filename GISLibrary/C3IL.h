#pragma once

struct C3IL 
{
public:
	C3IL();
	C3IL(const C3IL& other);
	C3IL(int x, int y, int z);
	virtual ~C3IL();

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

	/*
	* Coordinate in Z axis
	* Format		: b24
	* Description	: Z-coordinate (depth or height)
	*/
	int m_zcoo;

public:
	virtual C3IL operator=(const C3IL& other);

public:
	static int GetSize();
};