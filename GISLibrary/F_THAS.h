#pragma once
#include "Field.h"

struct  THAS;
class F_THAS : public Field
{
public:
	F_THAS();
	F_THAS(const F_THAS& other);
	virtual ~F_THAS();

	std::vector<THAS*> m_arr;

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	bool WriteField(CFile* file) { return true; }
	int GetFieldLength();

	void Insert(RecordName name, int taui);
	void Insert(int rcnm, int rcid, int taui);
	void Insert(GISLibrary::RCNM rcnm, int rcid, int taui);

	virtual F_THAS* Clone() const;
};