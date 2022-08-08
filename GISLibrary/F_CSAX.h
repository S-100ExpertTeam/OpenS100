#pragma once

#include "Field.h"

#include <list>

struct CSAX;

class F_CSAX : Field
{
public:
	F_CSAX();
	virtual ~F_CSAX();

public:
	std::list<CSAX*> m_arr;

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	bool WriteField(CFile* file);

	int GetFieldLength();
};