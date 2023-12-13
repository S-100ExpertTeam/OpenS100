#pragma once
#include "Field.h"

class F_CRSH : public Field
{
public:
	F_CRSH();
	F_CRSH(const F_CRSH& other);
	virtual ~F_CRSH();
public:
	/*
	* CRS Index
	* Format		: b11
	* Description	: Internal identifier of the CRS
	* (Used for identifying the vertical CRS in C3DI or C3DF)
	*/
	int m_crix;
	/*
	* CRS Type
	* Format		: b11
	* see table
	*/
	int m_crst;
	/*
	* Coordinate System Type
	* Format		: b11
	* {1} - Ellipsoidal CS
	* {2} - Cartesian CS
	* {3} - Vertical CS
	*/
	int m_csty;
	/*
	* CRS Name
	* Format		: A
	* Name of the Coordinate Reference System
	*/
	CString m_crnm;
	/*
	* CRS Identifier
	* Format		: A
	* Identifier of the CRS from an external source.
	* Empty if not defined by reference
	*/
	CString m_crsi;
	/*
	* CRS Source
	* Format		: b11
	* {1} - IHO CRS Register
	* {2} - EPSG
	* {254} - Other Source
	* {255} - Not Applicable
	*/
	int m_crss;
	/*
	* CRS Source Information
	* Format		: A
	* Information about the CRS source if CRSS = ¡®Other Source¡¯
	*/
	CString m_scri;

public:
	void ReadField(BYTE *&buf);
	bool WriteField(CFile* file);

	int GetFieldLength();

	virtual F_CRSH* Clone() const;
};

