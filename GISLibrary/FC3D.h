#pragma once
struct FC3D
{
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
	static int GetSize();
};