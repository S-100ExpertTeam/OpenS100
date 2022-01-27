#include "StdAfx.h"
#include "F_CSID.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_CSID::F_CSID(void)
{
	m_name.RCNM = 15;
	m_name.RCID = 1;
	m_ncrc = 0;
}

F_CSID::~F_CSID(void)
{

}

void F_CSID::ReadField(BYTE *&buf)
{
	m_name.RCNM = *(buf++);
	m_name.RCID = buf2uint(buf, 4);
	m_ncrc = *(buf++);
}

int F_CSID::GetFieldLength()
{
	int len = 0;
	len += m_name.GetLength();
	len += 1;
	return ++len;
}