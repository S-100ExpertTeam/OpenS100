#pragma once
#include "R_VectorRecord.h"
#include "F_MRID.h"

class F_INAS;
class F_COCC;
class F_C2IL;
class F_C3IL;
// Multi Point Record
class R_MultiPointRecord : public R_VectorRecord
{
public:
	R_MultiPointRecord();
	virtual ~R_MultiPointRecord();

public:
	F_MRID m_mrid;
	std::list<F_INAS*> m_inas;
	F_COCC* m_cocc = nullptr; // Only Update
	std::list<F_C2IL*> m_c2il;
	std::list<F_C3IL*> m_c3il;

public:
	BOOL ReadRecord(DRDirectoryInfo *dir, BYTE*& buf);
	int GetRCID();
	std::wstring GetRCIDasWstring();
};