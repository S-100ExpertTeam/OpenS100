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
	F_CCOC*            m_ccoc = nullptr;
	std::list<F_CUCO*> m_cuco;

public:
	BOOL ReadRecord(DRDirectoryInfo *dir, BYTE*& buf);
	bool WriteRecord(CFile* file);
	RecordName GetRecordName();

	int GetRCID();
	std::string GetRCIDasString(std::string prefix = "");
	std::wstring GetRCIDasWstring();

	void SetRCID(int rcid);

	void InsertCurve(int rcnm, int rcid, int ornt);

	std::vector<CUCO*> GetAllCUCO();
};