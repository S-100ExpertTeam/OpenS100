#pragma once
#include "Field.h"
#include "RecordName.h"

#include <vector>

struct PTAS;
class F_PTAS : public Field
{
public:
	F_PTAS();
	F_PTAS(const F_PTAS& other);
	virtual ~F_PTAS();

public:
	std::vector<PTAS*> m_arr;

public:
	virtual F_PTAS operator=(const F_PTAS& other);

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	bool WriteField(CFile* file);
	int GetFieldLength();

	void Insert(RecordName name, int topi);
	void Insert(int rcnm, int rcid, int topi);
	void Insert(GISLibrary::RCNM rcnm, int rcid, int topi);
};