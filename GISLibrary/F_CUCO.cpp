#include "stdafx.h"
#include "F_CUCO.h"
#include "CUCO.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_CUCO::F_CUCO(void)
{

}

F_CUCO::F_CUCO(const F_CUCO& other)
{
	for (const auto& iter : other.m_arr)
		Insert(iter->m_name, iter->m_ornt);
}

F_CUCO::~F_CUCO(void)
{
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		CUCO *cuco = *itor;
		delete cuco;
	}
}

void F_CUCO::ReadField(BYTE *&buf)
{
	while(*buf != 0x1E){
		CUCO *cuco = new CUCO();
		cuco->m_name.RCNM = *(buf++);
		cuco->m_name.RCID = buf2uint(buf, 4);
		cuco->m_ornt = *(buf++);
		m_arr.push_back(cuco);
	}
}

void F_CUCO::ReadField(BYTE *&buf, int loopCnt)
{
	for(int i = 0; i < loopCnt; i++){
		CUCO *cuco = new CUCO();
		cuco->m_name.RCNM = *(buf++);
		cuco->m_name.RCID = buf2uint(buf, 4);
		cuco->m_ornt = *(buf++);
		m_arr.push_back(cuco);
	}
}

bool F_CUCO::WriteField(CFile* file)
{
	for (auto i = m_arr.begin(); i != m_arr.end(); i++)
	{
		file->Write(&(*i)->m_name.RCNM, 1);
		file->Write(&(*i)->m_name.RCID, 4);
		file->Write(&(*i)->m_ornt, 1);
	}

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

int F_CUCO::GetFieldLength()
{
	int len = 0;
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		CUCO *cuco = *itor;
		len += cuco->m_name.GetLength();
		len += 1;
	}
	return ++len;
}

void F_CUCO::Insert(RecordName name, int ornt)
{
	CUCO* cuco = new CUCO();
	cuco->m_name = name;
	cuco->m_ornt = ornt;
	m_arr.push_back(cuco);
}

void F_CUCO::Insert(int rcnm, int rcid, int ornt)
{
	CUCO* cuco = new CUCO();
	cuco->m_name.RCNM = rcnm;
	cuco->m_name.RCID = rcid;
	cuco->m_ornt = ornt;
	m_arr.push_back(cuco);
}

void F_CUCO::Insert(GISLibrary::RCNM rcnm, int rcid, int ornt)
{
	CUCO* cuco = new CUCO();
	cuco->m_name.RCNM = (int)rcnm;
	cuco->m_name.RCID = rcid;
	cuco->m_ornt = ornt;
	m_arr.push_back(cuco);
}

