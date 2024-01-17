#pragma once

struct C2IL
{
public:
	C2IL();
	C2IL(const C2IL& other);
	C2IL(int m_ycoo, int m_xcoo);
	virtual ~C2IL();

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

public:
	virtual C2IL operator=(const C2IL& other);

public:
	static int GetSize();
};