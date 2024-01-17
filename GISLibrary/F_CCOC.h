#pragma once
#include "Field.h"

class F_CCOC :
	public Field
{
public:
	F_CCOC();
	F_CCOC(const F_CCOC& other);
	virtual ~F_CCOC();

public:
	/*
	Coordinate Update Instruction
	b11
	{ 1 } -Insert
	{ 2 } -Delete
	{ 3 } -Modify
	*/
	int m_ccui;
	/*
	Coordinate Index
	b12
	Index(position) of the addressed coordinate tuple within the coordinate field(s) of the target record
	*/
	int m_ccix;
	/*
	Number of Coordinates
	b12
	Number of coordinate tuples in the coordinate field(s) of the update record
	*/
	int m_ncco;
	static int GetSize();

public:
	virtual F_CCOC operator=(const F_CCOC& other);

public:
	void ReadField(BYTE *&buf);
	bool WriteField(CFile* file) { return true; };
	int GetFieldLength();
};

