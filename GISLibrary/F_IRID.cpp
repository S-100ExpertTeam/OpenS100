#include "stdafx.h"
#include "F_IRID.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_IRID::F_IRID(void)
{
	m_name.RCNM = 0;
	m_name.RCID = 0;
	m_nitc = 0;
	m_rver = 0;
	m_ruin = 0;
}

F_IRID::~F_IRID(void)
{

}

void F_IRID::ReadField(BYTE *&buf)
{
	m_name.RCNM = *(buf++);
	m_name.RCID = buf2uint(buf, 4);
	m_nitc = buf2uint(buf, 2);
	m_rver = buf2uint(buf, 2);
	m_ruin = *(buf++);
}

int F_IRID::GetFieldLength()
{
	int len = 0;
	len += m_name.GetLength();
	len += 2;
	len += 2;
	len++;
	return ++len;
}