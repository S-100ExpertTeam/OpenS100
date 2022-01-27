#include "stdafx.h"
#include "F_C2IL.h"
#include "IC2D.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_C2IL::F_C2IL()
{

}

F_C2IL::~F_C2IL()
{
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		delete *itor;
	}
	m_arr.clear();
}

void F_C2IL::ReadField(BYTE *&buf)
{
	while (*buf != 0x1E)
	{
		IC2D *cont = new IC2D();
		cont->m_ycoo = buf2int(buf, 4);
		cont->m_xcoo = buf2int(buf, 4);
		m_arr.push_back(cont);
	}
}

void F_C2IL::ReadField(BYTE *&buf, int loopCnt)
{
	for (int i = 0; i < loopCnt; i++)
	{
		IC2D *cont = new IC2D();
		cont->m_ycoo = buf2int(buf, 4);
		cont->m_xcoo = buf2int(buf, 4);
		m_arr.push_back(cont);
	}
}

int F_C2IL::GetFieldLength()
{
	unsigned len = 0;
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		len += IC2D::GetSize();
	}
	return ++len;
}