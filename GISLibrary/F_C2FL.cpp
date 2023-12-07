#include "stdafx.h"
#include "F_C2FL.h"
#include "FC2D.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_C2FL::F_C2FL()
{

}

F_C2FL::~F_C2FL()
{
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
		delete *itor;
}

void F_C2FL::ReadField(BYTE *&buf)
{
	while (*buf != 0x1E)
	{
		FC2D *cont = new FC2D();
		cont->m_ycoo = buf2double(buf, 8);
		cont->m_xcoo = buf2double(buf, 8);
		m_arr.push_back(cont);
	}
}

void F_C2FL::ReadField(BYTE *&buf, int loopCnt)
{
	for (int i = 0; i < loopCnt; i++)
	{
		FC2D *cont = new FC2D();
		cont->m_ycoo = buf2double(buf, 8);
		cont->m_xcoo = buf2double(buf, 8);
		m_arr.push_back(cont);
	}
}

int F_C2FL::GetFieldLength()
{
	unsigned len = 0;
	len++;
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		len += FC2D::GetSize();
	}
	return ++len;
}

void F_C2FL::Insert(int xcoo, int ycoo)
{
	FC2D* cont = new FC2D();
	cont->m_ycoo = ycoo;
	cont->m_xcoo = xcoo;
	m_arr.push_back(cont);
}

