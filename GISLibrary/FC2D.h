#pragma once

struct FC2D {

public:
	FC2D();
	FC2D(const FC2D& other);
	FC2D(double ycoo, double xcoo);
	virtual ~FC2D();

public:
	/*
	* Coordinate in Y axis
	* Format		: b48
	* Description	: Y-coordinate or latitude
	*/
	double m_ycoo;
	/*
	* Coordinate in X axis
	* Format		: b24
	* Description	: X-coordinate or longitude
	*/
	double m_xcoo;

public:
	virtual FC2D operator=(const FC2D& other);

public:
	static int GetSize();
};