#pragma once
#include "BinaryWriter.h"
#include <string>
#include "Field.h"

class F_GDAT :
	public Field
{
public:
	F_GDAT();
	F_GDAT(const F_GDAT& other);
	virtual ~F_GDAT();

public:
	/*
	* Datum Name
	* A
	* Name of the geodetic datum
	*/
	std::wstring m_dtnm;
	/*
	* Ellipsoid Name
	* A
	* Name of the ellipsoid
	*/
	std::wstring m_elnm;
	/*
	* Ellipsoid semi major axis
	* b48
	* Semi major axis of the ellipsoid in metre
	*/
	double m_esma;
	/*
	* Ellipsoid second parameter type
	* b11
	* {1} - Semi minor axis in metres
	* {2} - Inverse Flattening
	*/
	int m_espt;
	/*
	* Ellipsoid second parameter
	* b48
	* The second defining parameter of the ellipsoid
	*/
	double m_espm;
	/*
	* Central Meridian Name
	* A
	* Name of the central meridian
	*/
	std::wstring m_cmnm;
	/*
	* Central Meridian Greenwich Longitude
	* b48
	* Greenwich longitude of the central meridian in degrees
	*/
	double m_cmgl;

public:
	virtual F_GDAT operator=(const F_GDAT& other);

public:
	void ReadField(BYTE *&buf);
	bool WriteField(s100::io::BinaryWriter* file) { return true; }
	int GetFieldLength();
};