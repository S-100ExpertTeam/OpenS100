#pragma once

struct IC2D
{
public:
	IC2D();
	IC2D(int m_ycoo, int m_xcoo);
	virtual ~IC2D();

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
	static int GetSize();
};