#pragma once
#include "R_VectorRecord.h"
#include "F_CCID.h"

#include <list>

class F_CCOC;
class F_CUCO;
class F_INAS;
class DRDirectoryInfo; 
// Composite Curve Record
class R_CompositeRecord : public R_VectorRecord
{
public:
	R_CompositeRecord();
	virtual ~R_CompositeRecord();

public:
	F_CCID             m_ccid;
	std::list<F_INAS*> m_inas;
	F_CCOC*            m_ccoc = nullptr;
	std::list<F_CUCO*> m_cuco;

public:
	BOOL ReadRecord(DRDirectoryInfo *dir, BYTE*& buf);
	int GetRCID();
	std::wstring GetRCIDasWstring();
};