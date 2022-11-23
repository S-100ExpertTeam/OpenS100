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
	virtual ~R_DSCRS();

public:
	F_CSID m_csid;
	std::list<F_CRSH*> m_crsh;
	F_CSAX* m_csax = nullptr;
	//F_PROJ* m_proj = nullptr;
	//F_GDAT* m_gdat = nullptr;
	F_VDAT* m_vdat = nullptr;

public:
	BOOL ReadRecord(DRDirectoryInfo *dir, BYTE*& buf);
	bool WriteRecord(CFile* file);

	void Init();
	void Release();
};