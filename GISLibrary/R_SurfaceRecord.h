#pragma once

#include "R_VectorRecord.h"
#include "F_SRID.h"

#include <vector>

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
	std::list<F_RIAS*> m_rias; 

public:
	BOOL ReadRecord(DRDirectoryInfo *dir, BYTE*& buf);
	bool WriteRecord(CFile* file);
	RecordName GetRecordName();

	int GetRCID();
	std::string GetRCIDasString(std::string prefix = "");
	std::wstring GetRCIDasWstring();
	
	void SetRCID(int rcid);

	void InsertRing(int rcnm, int rcid, int usag, int ornt);

	std::vector<RIAS*> GetAllRIAS();
};