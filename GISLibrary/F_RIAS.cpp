#include "stdafx.h"
#include "F_RIAS.h"
#include "RIAS.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_RIAS::F_RIAS(void)
{

}

F_RIAS::F_RIAS(const F_RIAS& other)
{
	for (const auto& iter : other.m_arr)
		Insert(iter->m_name, iter->m_ornt, iter->m_usag, iter->m_raui);
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

void F_RIAS::Insert(RecordName name, int ornt, int usag, int raui)
{
	RIAS* rias = new RIAS();
	rias->m_name = name;
	rias->m_ornt = ornt;
	rias->m_usag = usag;
	rias->m_raui = raui;
	m_arr.push_back(rias);
}

void F_RIAS::Insert(int rcnm, int rcid, int ornt, int usag, int raui)
{
	RIAS* rias = new RIAS();
	rias->m_name.RCNM = rcnm;
	rias->m_name.RCID = rcid;
	rias->m_ornt = ornt;
	rias->m_usag = usag;
	rias->m_raui = raui;
	m_arr.push_back(rias);
}

void F_RIAS::Insert(GISLibrary::RCNM rcnm, int rcid, int ornt, int usag, int raui)
{
	RIAS* rias = new RIAS();
	rias->m_name.RCNM = (int)rcnm;
	rias->m_name.RCID = rcid;
	rias->m_ornt = ornt;
	rias->m_usag = usag;
	rias->m_raui = raui;
	m_arr.push_back(rias);
}

F_RIAS* F_RIAS::Clone() const
{
	F_RIAS* f_rias = new F_RIAS();
	for (const auto& iter : m_arr)
	{
		RIAS* rias = new RIAS();
		rias->m_name = iter->m_name;
		rias->m_ornt = iter->m_ornt;
		rias->m_usag = iter->m_usag;
		rias->m_raui = iter->m_raui;
		f_rias->m_arr.push_back(rias);
	}

	return f_rias;
}
