#include "StdAfx.h"
#include "F_INAS.h"
#include "ATTR.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_INAS::F_INAS()
{
	m_name.RCNM = 0;
	m_name.RCID = 0;
	m_niac = 0;
	m_narc = 0;
	m_iuin = 0;	
}

F_INAS::~F_INAS()
{
	POSITION pos = m_arr.GetHeadPosition();

	while (pos != NULL)
	{
		ATTR* attr = m_arr.GetNext(pos);
		delete attr;
	}
}

int F_INAS::GetSize()
{
	return 10;
}

void F_INAS::ReadField(BYTE *&buf)
{
	m_name.RCNM = *(buf++);
	m_name.RCID = buf2uint(buf, 4);
	m_niac = buf2uint(buf, 2);
	m_narc = buf2uint(buf, 2);
	m_iuin = *(buf++);
	while (*buf != 0x1E)
	{
		ATTR *attr = new ATTR();
		attr->m_natc = buf2uint(buf, 2);
		attr->m_atix = buf2uint(buf, 2);
		attr->m_paix = buf2uint(buf, 2);
		attr->m_atin = *(buf++);
		buf2charArr(attr->m_atvl, buf);
		m_arr.AddTail(attr);
	}
}

void F_INAS::ReadField(BYTE *&buf, int loopCnt)
{
	m_name.RCNM = *(buf++);
	m_name.RCID = buf2uint(buf, 4);
	m_niac = buf2uint(buf, 2);
	m_narc = buf2uint(buf, 2);
	m_iuin = *(buf++);
	for (int i = 0; i < loopCnt; i++)
	{
		ATTR *attr = new ATTR();
		attr->m_natc = buf2uint(buf, 2);
		attr->m_atix = buf2uint(buf, 2);
		attr->m_paix = buf2uint(buf, 2);
		attr->m_atin = *(buf++);
		buf2charArr(attr->m_atvl, buf);
		m_arr.AddTail(attr);
	}
}

BOOL F_INAS::Save(CFile *file)
{
	file->Write(&m_name.RCNM, 1);
	file->Write(&m_name.RCID, 4);
	file->Write(&m_niac, 2);
	file->Write(&m_narc, 2);
	file->Write(&m_iuin, 1);
	POSITION pos = m_arr.GetHeadPosition();
	while (pos != NULL)
	{
		ATTR *attr = m_arr.GetNext(pos);
		file->Write(&attr->m_natc, 2);
		file->Write(&attr->m_atix, 2);
		file->Write(&attr->m_paix, 2);
		file->Write(&attr->m_atin, 1);
		CT2CA outputString(attr->m_atvl, CP_UTF8);
		file->Write(outputString, (UINT)::strlen(outputString));
		file->Write(&NonPrintableCharacter::unitTerminator, 1);
	}
	file->Write(&NonPrintableCharacter::fieldTerminator, 1);
	return TRUE;
}

int F_INAS::GetFieldLength()
{
	int len = 0;
	len += F_INAS::GetSize();
	POSITION pos = m_arr.GetHeadPosition();
	while (pos != NULL)
	{
		ATTR *attr = m_arr.GetNext(pos);
		len += ATTR::GetOffsetToATVL();
		CT2CA outputString(attr->m_atvl, CP_UTF8);
		len += (int)::strlen(outputString) + 1;
	}
	return ++len;
}
