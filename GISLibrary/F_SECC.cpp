#include "stdafx.h"
#include "F_SECC.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_SECC::F_SECC()
{
	m_seui = new int();
	m_seix = new int();
	m_nseg = new int();
}

F_SECC::~F_SECC()
{

}

void F_SECC::ReadField(BYTE *&buf)
{
	*m_seui = *(buf++);
	*m_seix = buf2uint(buf, 2);
	*m_nseg = buf2uint(buf, 2);
}

void F_SECC::ReadField(BYTE *&buf, int loopCnt)
{
	*m_seui = *(buf++);
	*m_seix = buf2uint(buf, 2);
	*m_nseg = buf2uint(buf, 2);
}

BOOL F_SECC::Save(CFile *file)
{
	file->Write(m_seui, 1);
	file->Write(m_seix, 2);
	file->Write(m_nseg, 2);
	file->Write(&NonPrintableCharacter::fieldTerminator, 1);
	return TRUE;
}

int F_SECC::GetFieldLength()
{
	int len = 0;
	len += 1;
	len += 2;
	len += 2;
	return ++len;
}