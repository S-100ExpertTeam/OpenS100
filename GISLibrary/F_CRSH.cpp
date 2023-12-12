#include "stdafx.h"
#include "F_CRSH.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_CRSH::F_CRSH(void)
{
	m_crix = 0;
	m_crst = 0;
	m_csty = 0;
	m_crnm = "";
	m_crsi = "";
	m_crss = 0;
	m_scri = "";
}

F_CRSH::F_CRSH(const F_CRSH& other)
{
	m_crix = other.m_crix;
	m_crst = other.m_crst;
	m_csty = other.m_csty;
	m_crnm = other.m_crnm;
	m_crsi = other.m_crsi;
	m_crss = other.m_crss;
	m_scri = other.m_scri;
}

F_CRSH::~F_CRSH(void)
{

}

void F_CRSH::ReadField(BYTE *&buf)
{
	m_crix = *(buf++);
	m_crst = *(buf++);
	m_csty = *(buf++);
	buf2charArr(m_crnm, buf);
	buf2charArr(m_crsi, buf);
	m_crss = *(buf++);
	buf2charArr(m_scri, buf);
}

bool F_CRSH::WriteField(CFile* file)
{
	file->Write(&m_crix, 1);
	file->Write(&m_crst, 1);
	file->Write(&m_csty, 1);

	CT2CA outputStringCRNM(m_crnm, CP_UTF8);
	file->Write(outputStringCRNM, (UINT)::strlen(outputStringCRNM));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	CT2CA outputStringCRSI(m_crsi, CP_UTF8);
	file->Write(outputStringCRSI, (UINT)::strlen(outputStringCRSI));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	file->Write(&m_crss, 1);
	
	CT2CA outputStringSCRI(m_scri, CP_UTF8);
	file->Write(outputStringSCRI, (UINT)::strlen(outputStringSCRI));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

int F_CRSH::GetFieldLength()
{
	int len = 0;

	len += 1;
	len += 1;
	len += 1;
	len += m_crnm.GetLength() + 1;
	len += m_crsi.GetLength() + 1;
	len += 1;
	len += m_scri.GetLength() + 1;
	return ++len;
}