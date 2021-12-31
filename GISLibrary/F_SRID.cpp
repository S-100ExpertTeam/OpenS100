#include "StdAfx.h"
#include "F_SRID.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_SRID::F_SRID(void)
{
	m_name.RCNM = 0;
	m_name.RCID = 0;
	m_rver = 0;
	m_ruin = 0;
}

F_SRID::~F_SRID(void)
{

}

void F_SRID::ReadField(BYTE *&buf)
{
	m_name.RCNM = *(buf++);
	m_name.RCID = buf2uint(buf, 4);
	m_rver = buf2uint(buf, 2);
	m_ruin = *(buf++);
}

BOOL F_SRID::Save(CFile *file)
{
	file->Write(&m_name.RCNM, 1);
	file->Write(&m_name.RCID, 4);
	file->Write(&m_rver, 2);
	file->Write(&m_ruin, 1);
	file->Write(&NonPrintableCharacter::fieldTerminator, 1);
	return TRUE;
}

int F_SRID::GetFieldLength()
{
	int len = 0;
	len += m_name.GetLength();
	len += 2;
	len += 1;
	return ++len;
}