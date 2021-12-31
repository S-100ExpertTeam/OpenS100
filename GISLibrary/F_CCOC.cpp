#include "stdafx.h"
#include "F_CCOC.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_CCOC::F_CCOC()
{

}

F_CCOC::~F_CCOC()
{

}

int F_CCOC::GetSize()
{
	return 5;
}

void F_CCOC::ReadField(BYTE *&buf)
{
	m_ccui = buf2uint(buf, 1);
	m_ccix = buf2uint(buf, 2);
	m_ncco = buf2uint(buf, 2);
}

BOOL F_CCOC::Save(CFile *file)
{
	file->Write(&m_ccui, 1);
	file->Write(&m_ccix, 2);
	file->Write(&m_ncco, 2);

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);
	return TRUE;
}

int F_CCOC::GetFieldLength()
{
	int len = 0;
	len += 1;
	len += 2;
	len += 2;
	return ++len;
}