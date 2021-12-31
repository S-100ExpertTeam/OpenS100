#include "StdAfx.h"
#include "F_SEGH.h"
#include "NonPrintableCharacter.h"

F_SEGH::F_SEGH(void)
{
	m_intp = 0;
	m_circ = 0;
	m_ycoo = 0;
	m_xcoo = 0;
	m_dist = 0;
	m_disu = 0;
	m_sbrg = 0;
	m_angl = 0;
}

F_SEGH::~F_SEGH(void)
{

}

void F_SEGH::ReadField(BYTE *&buf)
{
	m_intp = *(buf++);
}

BOOL F_SEGH::Save(CFile *file)
{
	file->Write(&m_intp, 1);
	file->Write(&NonPrintableCharacter::fieldTerminator, 1);
	return TRUE;
}

int F_SEGH::GetFieldLength()
{
	int len = 0;
	len += 1;
	return ++len;
}