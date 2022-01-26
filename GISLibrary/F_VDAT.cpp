#include "StdAfx.h"
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

F_VDAT::~F_VDAT()
{

}

void F_VDAT::ReadField(BYTE *&buf)
{
	buf2charArr(m_dtnm, buf);
	buf2charArr(m_dtid, buf);
	m_dtsr = *(buf++);
	buf2charArr(m_scri, buf);
}

int F_VDAT::GetFieldLength()
{
	int len = 0;
	len += m_dtnm.GetLength()+1;
	len += m_dtid.GetLength()+1;
	len++;
	len += m_scri.GetLength()+1;
	return ++len;
}