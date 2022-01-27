#pragma once
#include "Field.h"
#include "RecordName.h"

class F_IRID : Field
{

public:
	F_IRID();
	virtual ~F_IRID();

public:
	/*
	* Referenced Record name/identifier
	*/
	RecordName m_name;	

	/*
	* Numeric Information Type Code
	* Format		: b12
	* Description	: A valid information type code as defined in the ITCS field of the Dataset General Information Record
	*/
	int m_nitc;

	/*
	* Record version
	* Format		: b12
	* Description	: RVER contains the serial number of the record edition
	*/
	int m_rver;

	/*
	* Record update instruction
	* Format		: b11
	* {1} - Insert
	* {2} - Delete
	* {3} - Modify
	*/
	int m_ruin;
	CString name = L"";

public:
	void ReadField(BYTE *&buf);
	int GetFieldLength();
};