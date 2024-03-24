#pragma once
struct FC3D
{
public:
	FC3D();
	FC3D(const FC3D& other);
	FC3D(double ycoo, double xcoo, double zcoo);
	virtual ~FC3D();

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
	/*
	* Coordinate in Z axis
	* Format		: b48
	* Description	: Z-coordinate (depth or height)
	*/
	double m_zcoo;

public:
	virtual FC3D operator=(const FC3D& other);

public:
	static int GetSize();
};