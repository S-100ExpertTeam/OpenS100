#pragma once
#include "Field.h"

struct CUCO;
class F_CUCO : public Field
{
public:
	F_CUCO();
	F_CUCO(const F_CUCO& other);
	virtual ~F_CUCO();
	
public:
	std::list<CUCO*> m_arr;

public:
	virtual F_CUCO operator=(const F_CUCO& other);

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	bool WriteField(CFile* file);

	int GetFieldLength();

	void Insert(RecordName name, int ornt);
	void Insert(int rcnm, int rcid, int ornt);
	void Insert(GISLibrary::RCNM rcnm, int rcid, int ornt);
};