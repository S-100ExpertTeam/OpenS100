#include "stdafx.h"
#include "F_FOID.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_FOID::F_FOID(void)
{
	m_objName.m_agen = 0;
	m_objName.m_fidn = 0;
	m_objName.m_fids = 0;
}


F_FOID::~F_FOID(void)
{

}

void F_FOID::ReadField(BYTE *&buf)
{
	m_objName.m_agen = buf2uint(buf, 2);
	m_objName.m_fidn = buf2uint(buf, 4);
	m_objName.m_fids = buf2uint(buf, 2);
}

bool F_FOID::WriteField(CFile* file)
{
	file->Write(&m_objName.m_agen, 2);
	file->Write(&m_objName.m_fidn, 4);
	file->Write(&m_objName.m_fids, 2);

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

int F_FOID::GetFieldLength()
{
	int len = 0;

	len += m_objName.GetLength();
	return ++len;
}