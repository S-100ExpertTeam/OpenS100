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
	void ReadField(BYTE *&buf);
	bool WriteField(CFile* file);
	int GetFieldLength();

	virtual F_VDAT* Clone() const;
};