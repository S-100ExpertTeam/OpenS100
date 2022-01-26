#include "stdafx.h"
#include "F_C3FL.h"
#include "FC3D.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_C3FL::F_C3FL()
{

}

F_C3FL::~F_C3FL()
{
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
		delete *itor;
}

void F_C3FL::ReadField(BYTE *&buf)
{
	m_vcid = *(buf++);
	while (*buf != 0x1E)
	{
		FC3D *cont = new FC3D();
		cont->m_ycoo = buf2double(buf, 8);
		cont->m_xcoo = buf2double(buf, 8);
		cont->m_zcoo = buf2double(buf, 8);

		m_arr.push_back(cont);
	}
}

void F_C3FL::ReadField(BYTE *&buf, int loopCnt)
{
	m_vcid = *(buf++);
	for (int i = 0; i < loopCnt; i++)
	{
		FC3D *cont = new FC3D();
		cont->m_ycoo = buf2double(buf, 8);
		cont->m_xcoo = buf2double(buf, 8);
		cont->m_zcoo = buf2double(buf, 8);

		m_arr.push_back(cont);
	}
}

int F_C3FL::GetFieldLength()
{
	int len = 0;
	len++; 
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		len += FC3D::GetSize();
	}
	return ++len;
}