#pragma once
#include "Field.h"

struct CSAX;
class F_CSAX : Field
{
public:
	F_CSAX();
	virtual ~F_CSAX();

public:
	CList<CSAX*> m_arr;

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	int GetFieldLength();
};