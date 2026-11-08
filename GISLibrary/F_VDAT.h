#pragma once
#include "BinaryWriter.h"
#include <string>
#include "Field.h"

class F_VDAT : public Field
{
public:
	F_VDAT();
	F_VDAT(const F_VDAT& other);
	virtual ~F_VDAT();

public:
	std::wstring m_dtnm;
	std::wstring m_dtid;
	int m_dtsr;
	std::wstring m_scri;

public:
	virtual F_VDAT operator=(const F_VDAT& other);

public:
	void ReadField(BYTE *&buf);
	bool WriteField(s100::io::BinaryWriter* file);
	int GetFieldLength();
};