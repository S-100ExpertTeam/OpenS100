#pragma once
#include "R_VectorRecord.h"
#include "F_SRID.h"

class F_INAS;
class F_RIAS;
// Surface Record
class R_SurfaceRecord : public R_VectorRecord
{
public:
	R_SurfaceRecord();
	virtual ~R_SurfaceRecord();

public:
	F_SRID m_srid;
	std::list<F_INAS*> m_inas;
	std::list<F_RIAS*> m_rias; 

public:
	BOOL ReadRecord(DRDirectoryInfo *dir, BYTE*& buf);
	bool WriteRecord(CFile* file);

	int GetRCID();
	std::wstring GetRCIDasWstring();
};