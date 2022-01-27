#include "StdAfx.h"
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

int F_FOID::GetFieldLength()
{
	int len = 0;

	len += m_objName.GetLength();
	return ++len;
}