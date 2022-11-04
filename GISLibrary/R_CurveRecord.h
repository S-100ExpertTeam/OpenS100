#pragma once
#include "R_VectorRecord.h"
#include "F_CRID.h"

#include <list>

class F_INAS;
class F_PTAS;
class F_SECC;
class F_SEGH;
class F_COCC;
class F_C2IL;
class DRDirectoryInfo;

// Curve Record
class R_CurveRecord : public R_VectorRecord
{
public:
	R_CurveRecord();
	virtual ~R_CurveRecord();

public:
	// B, U: <1> (4)
	F_CRID m_crid;

	// B, U : <0..*> (5\\*5)
	std::list<F_INAS*> m_inas;

	// B : <1> (*3)
	// U : <0..1> (*3)
	F_PTAS* m_ptas = nullptr;

	// B : Not used
	// U : <0..1> (3)
	F_SECC* m_secc = nullptr;

	// B : <1> (1)
	// U : <0..1> (1)
	F_SEGH* m_segh = nullptr;

	// B : Not used
	// U : <0..1> (3)
	F_COCC* m_cocc = nullptr;

	// B, U : <1..*> (*2)
	std::list<F_C2IL*> m_c2il;

public:
	BOOL ReadRecord(DRDirectoryInfo *dir, BYTE*& buf);
	bool WriteRecord(CFile* file);
	RecordName GetRecordName();

	int GetRCID();
	std::wstring GetRCIDasWstring();

	int GetPointCount();
};