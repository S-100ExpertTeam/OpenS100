#pragma once
#include "Field.h"

struct FC2D;
class F_C2FL : public Field
{
public:
	F_C2FL();
	F_C2FL(const F_C2FL& other);
	virtual ~F_C2FL();
	/*
	* Attribute Array
	*/
	std::list<FC2D*> m_arr;

public:
	void ReadField(BYTE*& buf);
	void ReadField(BYTE*& buf, int loopCnt);
	bool WriteField(CFile* file) { return true; }
	int GetFieldLength();

	void Insert(int xcoo, int ycoo);

	virtual F_C2FL* Clone() const;
};