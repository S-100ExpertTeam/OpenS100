#pragma once
#include "Field.h"
/*
* Field Tag: PROJ
* Field Name: Projection
*/
class F_PROJ :
	public Field
{
public:
	F_PROJ();
	virtual ~F_PROJ();

public:
	/*
	* Projection Method
	* b11
	* see table
	*/
	int m_prom;
	/*
	* Projection Parameter 1
	* b48
	* see table
	*/
	double m_prp1;
	/*
	* Projection Parameter 2
	* b48
	* see table
	*/
	double m_prp2;
	/*
	* Projection Parameter 3
	* b48
	* see table
	*/
	double m_prp3;
	/*
	* Projection Parameter 4
	* b48
	* see table
	*/
	double m_prp4;
	/*
	* Projection Parameter 5
	* b48
	* see table
	*/
	double m_prp5;
	/*
	* False Easting
	* b48
	* False easting (Units of measurement according to the coordinate axis ¡®Easting¡¯
	*/
	double m_feas;
	/*
	* False Northing
	* b48
	* False northing (Units of measurement according to the coordinate axis ¡®Northing¡¯
	*/
	double m_fnor;

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	int GetFieldLength();
	static int GetSize();
};