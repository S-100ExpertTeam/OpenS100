#include "stdafx.h"
#include "F_C3FL.h"
#include "FC3D.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_C3FL::F_C3FL()
{

}

F_C3FL::F_C3FL(const F_C3FL& other)
{
	for (const auto& iter : other.m_arr)
	{
		FC3D* cont = new FC3D();
		cont->m_ycoo = iter->m_ycoo;
		cont->m_xcoo = iter->m_xcoo;
		cont->m_zcoo = iter->m_zcoo;

		m_arr.push_back(cont);
	}

	m_vcid = other.m_vcid;
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

void F_C3FL::Insert(int xcoo, int ycoo, int zcoo)
{
	FC3D* cont = new FC3D();
	cont->m_ycoo = ycoo;
	cont->m_xcoo = xcoo;
	cont->m_zcoo = zcoo;

	m_arr.push_back(cont);
}

F_C3FL* F_C3FL::Clone() const
{
	F_C3FL* f_c3fl = new F_C3FL();
	f_c3fl->m_vcid = m_vcid;

	for (const auto& iter : m_arr)
	{
		FC3D* fc3d = new FC3D();
		fc3d->m_ycoo = iter->m_ycoo;
		fc3d->m_xcoo = iter->m_xcoo;
		fc3d->m_zcoo = iter->m_zcoo;

		f_c3fl->m_arr.push_back(fc3d);
	}

	return f_c3fl;
}

