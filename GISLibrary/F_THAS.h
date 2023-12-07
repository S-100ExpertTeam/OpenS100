#pragma once
#include "Field.h"

struct  THAS;
class F_THAS : public Field
{
public:
	F_THAS();
	virtual ~F_THAS();

	CList<THAS*> m_arr;

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	int GetFieldLength();

	void Insert(RecordName name, int taui);
	void Insert(int rcnm, int rcid, int taui);
	void Insert(GISLibrary::RCNM rcnm, int rcid, int taui);
};