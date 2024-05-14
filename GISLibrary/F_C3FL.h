#pragma once
#include "Field.h"

struct FC3D;
class F_C3FL : public Field
{
public:
	F_C3FL();
	F_C3FL(const F_C3FL& other);
	virtual ~F_C3FL();

public:
	/*
	* Vertical CRS Id
	* Format		: b11
	* Description	: Internal identifier of the Vertical CRS
	*/
	int m_vcid = 2;
	/*
	* Attribute Array
	*/
	std::list<FC3D*> m_arr;

public:
	virtual F_C3FL operator=(const F_C3FL& other);

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	bool WriteField(CFile* file) { return true; }
	int GetFieldLength();

	void Insert(int xcoo, int ycoo, int zcoo);
};