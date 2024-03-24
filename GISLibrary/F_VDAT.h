#pragma once
#include "Field.h"

class F_VDAT : public Field
{
public:
	F_VDAT();
	F_VDAT(const F_VDAT& other);
	virtual ~F_VDAT();

public:
	CString m_dtnm;
	CString m_dtid;
	int m_dtsr;
	CString m_scri;

public:
	virtual F_VDAT operator=(const F_VDAT& other);

public:
	void ReadField(BYTE *&buf);
	bool WriteField(CFile* file);
	int GetFieldLength();
};