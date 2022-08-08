#include "stdafx.h"
#include "F_RIAS.h"
#include "RIAS.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_RIAS::F_RIAS(void)
{

}

F_RIAS::~F_RIAS(void)
{
	for (auto itorParent = m_arr.begin(); itorParent != m_arr.end(); itorParent++)
	{
		RIAS* rias = *itorParent;
		delete rias;
	}
}

void F_RIAS::ReadField(BYTE *&buf)
{
	while(*buf != 0x1E){
		RIAS* rias = new RIAS();
		rias->m_name.RCNM = *(buf++);
		rias->m_name.RCID = buf2uint(buf, 4);
		rias->m_ornt = *(buf++);
		rias->m_usag = *(buf++);
		rias->m_raui = *(buf++);
		m_arr.push_back(rias);
	}
}

void F_RIAS::ReadField(BYTE *&buf, int loopCnt)
{
	for(int i = 0; i < loopCnt; i++)
	{
		RIAS* rias = new RIAS();
		rias->m_name.RCNM = *(buf++);
		rias->m_name.RCID = buf2uint(buf, 4);
		rias->m_ornt = *(buf++);
		rias->m_usag = *(buf++);
		rias->m_raui = *(buf++);
		m_arr.push_back(rias);
	}
}

bool F_RIAS::WriteField(CFile* file)
{
	for (auto i = m_arr.begin(); i != m_arr.end(); i++)
	{
		file->Write(&(*i)->m_name.RCNM, 1);
		file->Write(&(*i)->m_name.RCID, 4);
		file->Write(&(*i)->m_ornt, 1);
		file->Write(&(*i)->m_usag, 1);
		file->Write(&(*i)->m_raui, 1);
	}

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

int F_RIAS::GetFieldLength()
{
	int len = 0;
	for (auto itorParent = m_arr.begin(); itorParent != m_arr.end(); itorParent++)
	{
		RIAS* rias = *itorParent;
		len += rias->GetSize();
	}
	return ++len;
}