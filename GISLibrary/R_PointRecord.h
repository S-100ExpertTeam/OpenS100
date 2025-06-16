#pragma once

#include "R_VectorRecord.h"
#include "F_PRID.h"
#include "GM_Point.h"

#include <list>

class F_INAS;
class F_C2IT;
class F_C3IT;
class DRDirectoryInfo;

class R_PointRecord : 
	public R_VectorRecord
{
public:
	R_PointRecord();
	R_PointRecord(const R_PointRecord& other);
	virtual ~R_PointRecord();

public:
	F_PRID m_prid;
	F_C2IT* m_c2it = nullptr;
	F_C3IT* m_c3it = nullptr;

public:
	virtual R_PointRecord operator=(const R_PointRecord& other);

public:
	BOOL ReadRecord(DRDirectoryInfo *dir, BYTE*& buf);
	bool WriteRecord(CFile* file);
	RecordName GetRecordName();

	int GetRCID();
	std::string GetRCIDasString(std::string prefix = "");
	std::wstring GetRCIDasWstring();

	void SetRCID(int rcid);

	void SetC2IT(F_C2IT* value);
	void SetC2IT(int x, int y);
	void SetC3IT(F_C3IT* value);
	void SetC3IT(int x, int y, int z);

	GM::Point toGM_Point();
};