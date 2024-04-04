#include "stdafx.h"
#include "F_VDAT.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_VDAT::F_VDAT()
{
	m_dtnm = "";
	m_dtid = "";
	m_dtsr = 0;
	m_scri = "";
}

F_VDAT::F_VDAT(const F_VDAT& other) : Field(other)
{
	m_dtnm = other.m_dtnm;
	m_dtid = other.m_dtid;
	m_dtsr = other.m_dtsr;
	m_scri = other.m_scri;
}

F_VDAT::~F_VDAT()
{

}

F_VDAT F_VDAT::operator=(const F_VDAT& other)
{
	m_dtnm = other.m_dtnm;
	m_dtid = other.m_dtid;
	m_dtsr = other.m_dtsr;
	m_scri = other.m_scri;

	return *this;
}

void F_VDAT::ReadField(BYTE*& buf)
{
	buf2charArr(m_dtnm, buf);
	buf2charArr(m_dtid, buf);
	m_dtsr = *(buf++);
	buf2charArr(m_scri, buf);
}

bool F_VDAT::WriteField(CFile* file)
{
	CT2CA outputStringDTNM(m_dtnm, CP_UTF8);
	file->Write(outputStringDTNM, (UINT)::strlen(outputStringDTNM));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputStringDTID(m_dtid, CP_UTF8);
	file->Write(outputStringDTID, (UINT)::strlen(outputStringDTID));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	file->Write(&m_dtsr, 1);

	CT2CA outputStringSCRI(m_scri, CP_UTF8);
	file->Write(outputStringSCRI, (UINT)::strlen(outputStringSCRI));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);


	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

int F_VDAT::GetFieldLength()
{
	int len = 0;
	len += m_dtnm.GetLength() + 1;
	len += m_dtid.GetLength() + 1;
	len++;
	len += m_scri.GetLength() + 1;
	return ++len;
}

