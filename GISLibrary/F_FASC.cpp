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