#pragma once
#include "Field.h"
#include "RecordName.h"

struct RIAS;
class F_RIAS : public Field
{
public:
	F_RIAS();
	virtual ~F_RIAS();
	
public:
	std::list<RIAS*> m_arr;

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	bool WriteField(CFile* file);

	int GetFieldLength();

	void Insert(RecordName name, int ornt, int usag, int raui);
	void Insert(int rcnm, int rcid, int ornt, int usag, int raui);
	void Insert(GISLibrary::RCNM rcnm, int rcid, int ornt, int usag, int raui);
};