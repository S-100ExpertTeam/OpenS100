#include "stdafx.h"
#include "F_C3IT.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_C3IT::F_C3IT()
{

}

F_C3IT::~F_C3IT()
{

}

int F_C3IT::GetSize()
{
	return 13;
}

void F_C3IT::ReadField(BYTE *&buf)
{
	m_vcid = *(buf++);
	m_ycoo = buf2int(buf, 4);
	m_xcoo = buf2int(buf, 4);
	m_zcoo = buf2int(buf, 4);
}

BOOL F_C3IT::Save(CFile *file)
{
	file->Write(&m_vcid, 1);
	file->Write(&m_ycoo, 4);
	file->Write(&m_xcoo, 4);
	file->Write(&m_zcoo, 4);

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);
	
	return TRUE;
}

int F_C3IT::GetFieldLength()
{
	int len = 0;
	len += F_C3IT::GetSize();
	return ++len;
}
