#include "StdAfx.h"
#include "F_CRID.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_CRID::F_CRID(void)
{	
	m_name.RCNM = 0;
	m_name.RCID = 0;
	m_rver = 0;
	m_ruin = 0;
}

F_CRID::~F_CRID(void)
{

}

void F_CRID::ReadField(BYTE *&buf)
{
	m_name.RCNM = *(buf++);
	m_name.RCID = buf2uint(buf, 4);
	m_rver = buf2uint(buf, 2);
	m_ruin = *(buf++);
}

int F_CRID::GetFieldLength()
{
	int len = 0;
	len += m_name.GetLength();
	len += 2;
	len += 1;
	return ++len;
}