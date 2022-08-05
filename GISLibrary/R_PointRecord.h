#pragma once

#include "R_VectorRecord.h"
#include "F_PRID.h"

#include <list>

class F_INAS;
class F_C2IT;
class F_C3IT;
class DRDirectoryInfo;
// Point Record
class R_PointRecord : public R_VectorRecord
{
public:
	R_PointRecord();
	virtual ~R_PointRecord();

public:
	F_PRID m_prid;
	std::list<F_INAS*> m_inas; 
	F_C2IT* m_c2it = nullptr;
	F_C3IT* m_c3it = nullptr;

public:
	BOOL ReadRecord(DRDirectoryInfo *dir, BYTE*& buf);
	int GetRCID();
	std::wstring GetRCIDasWstring();
	void SetC2IT(F_C2IT* value);
	void SetC3IT(F_C3IT* value);
};