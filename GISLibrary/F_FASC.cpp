#include "stdafx.h"
#include "F_FASC.h"
#include "FASC.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_FASC::F_FASC(void)
{

}

F_FASC::~F_FASC(void)
{
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
		delete *itor;
}

void F_FASC::ReadField(BYTE *&buf)
{
	m_name.RCNM = *(buf++);
	m_name.RCID = buf2uint(buf, 4);
	m_nfac = buf2uint(buf, 2);
	m_narc = buf2uint(buf, 2);
	m_faui = *(buf++);
}

void F_FASC::ReadField(BYTE *&buf, int loopCnt)
{
	m_name.RCNM = *(buf++);
	m_name.RCID = buf2uint(buf, 4);
	m_nfac = buf2uint(buf, 2);
	m_narc = buf2uint(buf, 2);
	m_faui = *(buf++);
}

bool F_FASC::WriteField(CFile* file)
{
	file->Write(&m_name.RCNM, 1);
	file->Write(&m_name.RCID, 4);
	file->Write(&m_nfac, 2);
	file->Write(&m_narc, 2);
	file->Write(&m_faui, 1);

	for (auto i = m_arr.begin(); i != m_arr.end(); i++)
	{
		file->Write(&(*i)->m_natc, 2);
		file->Write(&(*i)->m_atix, 2);
		file->Write(&(*i)->m_paix, 2);
		file->Write(&(*i)->m_atin, 1);
		CT2CA outputString((*i) ->m_atvl, CP_UTF8);
		file->Write(outputString, (UINT)::strlen(outputString));
		file->Write(&NonPrintableCharacter::unitTerminator, 1);
	}

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}


int F_FASC::GetFieldLength()
{
	int len = 0;
	len += 10;
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		FASC *cont = *itor;
		len += FASC::GetSize();
		CT2CA outputString(cont->m_atvl, CP_UTF8);
		len += (int)::strlen(outputString) + 1;
	}
	return ++len;
}