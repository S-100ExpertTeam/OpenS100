#include "stdafx.h"
#include "F_COCC.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_COCC::F_COCC()
{

}

F_COCC::~F_COCC()
{

}

void F_COCC::ReadField(BYTE *&buf)
{
	m_coui = buf2uint(buf, 1);
	m_coix = buf2uint(buf, 2);
	m_ncor = buf2uint(buf, 2);
}

BOOL F_COCC::Save(CFile *file)
{
	file->Write(&m_coui, 1);
	file->Write(&m_coix, 2);
	file->Write(&m_ncor, 2);

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);
	return TRUE;
}

int F_COCC::GetFieldLength()
{
	int len = 0;
	len += 1;
	len += 2;
	len += 2;
	return ++len;
}