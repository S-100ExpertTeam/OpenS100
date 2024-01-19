#include "stdafx.h"
#include "F_C2FL.h"
#include "FC2D.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_C2FL::F_C2FL()
{

}

F_C2FL::F_C2FL(const F_C2FL& other)
{
	for (const auto& iter : other.m_arr)
	{
		FC2D* cont = new FC2D();
		cont->m_ycoo = iter->m_ycoo;
		cont->m_xcoo = iter->m_xcoo;
		m_arr.push_back(cont);
	}
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

F_C2FL* F_C2FL::Clone() const
{
	F_C2FL* f_c2fl = new F_C2FL();
	for (const auto& iter : m_arr)
	{
		FC2D* fc2d = new FC2D();
		fc2d->m_ycoo = iter->m_ycoo;
		fc2d->m_xcoo = iter->m_xcoo;
		f_c2fl->m_arr.push_back(fc2d);
	}

	return f_c2fl;
}

