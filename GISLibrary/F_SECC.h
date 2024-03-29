#pragma once
#include "Field.h"

class F_SECC :
	public Field
{
public:
	F_SECC();
	F_SECC(const F_SECC& other);
	virtual ~F_SECC();

public:
	/*
	* Segment update instruction
	* Format		: b11
	* {1} - Insert
	* {2} - Delete
	* {3} - Modify
	*/
	int m_seui;
	/*
	* Segment index
	* Format		: b12
	* Index (position) of the addressed segment in the target record
	*/
	int m_seix;
	/*
	* Number of segments
	* Format		: b12
	* Number of segments in the update record
	*/
	int m_nseg;

public:
	virtual F_SECC operator=(const F_SECC& other);

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	bool WriteField(CFile* file) { return true; };
	int GetFieldLength();
};