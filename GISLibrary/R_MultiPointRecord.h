#pragma once
#include "R_VectorRecord.h"
#include "F_MRID.h"

class F_INAS;
class F_COCC;
class F_C2IL;
class F_C3IL;

struct C2IL;
struct C3IL;

// Multi Point Record
class R_MultiPointRecord : public R_VectorRecord
{
public:
	R_MultiPointRecord();
	virtual ~R_MultiPointRecord();

public:
	F_MRID m_mrid;
	F_COCC* m_cocc = nullptr; // Only Update
	std::list<F_C2IL*> m_c2il;
	std::list<F_C3IL*> m_c3il;

public:
	BOOL ReadRecord(DRDirectoryInfo *dir, BYTE*& buf);
	bool WriteRecord(CFile* file);
	RecordName GetRecordName();

	int GetRCID();
	std::string GetRCIDasString(std::string prefix = "");
	std::wstring GetRCIDasWstring();

	void SetRCID(int rcid);
	void SetCOCC(int coui, int coix, int ncor);

	void InsertC3IL(int x, int y, int z);
	void InsertC3IL(int cmfx, int cmfy, int cmfz, SAFEARRAY* xcoo, SAFEARRAY* ycoo, SAFEARRAY* zcoo);

	void SetC3IL(int cmfx, int cmfy, int cmfz, SAFEARRAY* xcoo, SAFEARRAY* ycoo, SAFEARRAY* zcoo);
	void GetC3IL(double cmfx, double cmfy, double cmfz, SAFEARRAY** xcoo, SAFEARRAY** ycoo, SAFEARRAY** zcoo);

	std::vector<C2IL*> GetAllC2IL();
	std::vector<C3IL*> GetAllC3IL();
};