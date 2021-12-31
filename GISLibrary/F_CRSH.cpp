#include "StdAfx.h"
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

BOOL F_CRSH::Save(CFile *file)
{
	file->Write(&m_crix, 1);
	file->Write(&m_crst, 1);
	file->Write(&m_csty, 1);
	
	CT2CA outputString(m_crnm, CP_UTF8);
	file->Write(outputString, (UINT)::strlen(outputString));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);
	
	CT2CA outputString2(m_crsi, CP_UTF8);
	file->Write(outputString2, (UINT)::strlen(outputString2));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);
	file->Write(&m_crss, 1);

	CT2CA outputString3(m_scri, CP_UTF8);
	file->Write(outputString3, (UINT)::strlen(outputString3));
	file->Write(&NonPrintableCharacter::unitTerminator, 1);
	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return TRUE;
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