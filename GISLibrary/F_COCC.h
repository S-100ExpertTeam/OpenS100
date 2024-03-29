#pragma once
#include "Field.h"

class F_COCC :
	public Field
{
public:
	F_COCC();
	F_COCC(const F_COCC& other);
	virtual ~F_COCC();

public:
	/*
	Coordinate Update Instruction
		b11
	{ 1 } -Insert
	{ 2 } -Delete
	{ 3 } -Modify
	*/
	int m_coui;
	/* 
	Coordinate Index
	b12
	Index(position) of the addressed coordinate tuple within the coordinate field(s) of the target record
	*/
	int m_coix;
	/*
	Number of Coordinates
	b12
	Number of coordinate tuples in the coordinate field(s) of the update record
	*/
	int m_ncor;

public:
	virtual F_COCC operator=(const F_COCC& other);

public:
	void ReadField(BYTE *&buf);
	bool WriteField(CFile* file) { return true; };
	int GetFieldLength();
};

