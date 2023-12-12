#include "stdafx.h"
#include "F_THAS.h"
#include "THAS.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_THAS::F_THAS(void)
{

}

F_THAS::F_THAS(const F_THAS& other)
{
	for (const auto& iter : other.m_arr)
		Insert(iter->m_name, iter->m_taui);
}

F_THAS::~F_THAS(void)
{
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		delete* itor;
	}
}

void F_THAS::ReadField(BYTE *&buf)
{
	while(*buf != 0x1E)
	{
		THAS *thas = new THAS();
		thas->m_name.RCNM = *(buf++);
		thas->m_name.RCID = buf2uint(buf, 4);
		thas->m_taui = *(buf++);
	}
}

void F_THAS::ReadField(BYTE *&buf, int loopCnt)
{
	for(int i = 0; i < loopCnt; i++)
	{
		THAS *thas = new THAS();
		thas->m_name.RCNM = *(buf++);
		thas->m_name.RCID = buf2uint(buf, 4);
		thas->m_taui = *(buf++);
	}
}

int F_THAS::GetFieldLength()
{
	int len = 0;
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		THAS* thas = *itor;
		len += THAS::GetSize();
	}
	return ++len;
}

void F_THAS::Insert(RecordName name, int taui)
{
	THAS* thas = new THAS();
	thas->m_name = name;
	thas->m_taui = taui;
	m_arr.push_back(thas);
}

void F_THAS::Insert(int rcnm, int rcid, int taui)
{
	THAS* thas = new THAS();
	thas->m_name.RCNM = rcnm;
	thas->m_name.RCID = rcid;
	thas->m_taui = taui;
	m_arr.push_back(thas);
}

void F_THAS::Insert(GISLibrary::RCNM rcnm, int rcid, int taui)
{
	THAS* thas = new THAS();
	thas->m_name.RCNM = (int)rcnm;
	thas->m_name.RCID = rcid;
	thas->m_taui = taui;
	m_arr.push_back(thas);
}

