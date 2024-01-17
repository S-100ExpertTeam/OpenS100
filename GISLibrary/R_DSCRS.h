#pragma once
#include "Record.h"
#include "F_CSID.h"

class F_CRSH;
class F_CSAX;
class F_PROJ;
class F_GDAT;
class F_VDAT;
class DRDirectoryInfo;
// Dataset Coordinate Reference System Record
class R_DSCRS : Record
{
public:
	R_DSCRS();
	R_DSCRS(const R_DSCRS& other);
	virtual ~R_DSCRS();

public:
	F_CSID m_csid;
	std::list<F_CRSH*> m_crsh;
	F_CSAX* m_csax = nullptr;
	F_VDAT* m_vdat = nullptr;

public:
	virtual R_DSCRS operator=(const R_DSCRS& other);

public:
	BOOL ReadRecord(DRDirectoryInfo *dir, BYTE*& buf);
	bool WriteRecord(CFile* file);

	void Init();
	void Release();
};