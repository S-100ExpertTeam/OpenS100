#include "stdafx.h"
#include "F_C2IL.h"
#include "C2IL.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_C2IL::F_C2IL()
{

}

F_C2IL::F_C2IL(const F_C2IL& other)
{
	for (const auto& iter : other.m_arr)
	{
		C2IL* cont = new C2IL(*iter);
		m_arr.push_back(cont);
	}
}

F_C2IL::~F_C2IL()
{
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		delete *itor;
	}
	m_arr.clear();
}

F_C2IL F_C2IL::operator=(const F_C2IL& other)
{
	for (auto& iter : m_arr)
	{
		if (iter)
		{
			delete iter;
			iter = nullptr;
		}
	}
	m_arr.clear();

	for (const auto& iter : other.m_arr)
	{
		C2IL* cont = new C2IL(*iter);
		m_arr.push_back(cont);
	}

	return *this;
}

void F_C2IL::ReadField(BYTE *&buf)
{
	while (*buf != 0x1E)
	{
		C2IL *cont = new C2IL();
		cont->m_ycoo = buf2int(buf, 4);
		cont->m_xcoo = buf2int(buf, 4);
		m_arr.push_back(cont);
	}
}

void F_C2IL::ReadField(BYTE *&buf, int loopCnt)
{
	for (int i = 0; i < loopCnt; i++)
	{
		C2IL *cont = new C2IL();
		cont->m_ycoo = buf2int(buf, 4);
		cont->m_xcoo = buf2int(buf, 4);
		m_arr.push_back(cont);
	}
}

bool F_C2IL::WriteField(CFile* file)
{
	for (auto i = m_arr.begin(); i != m_arr.end(); i++)
	{
		file->Write(&(*i)->m_ycoo, 4);
		file->Write(&(*i)->m_xcoo, 4);
	}

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

int F_C2IL::GetFieldLength()
{
	unsigned len = 0;
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		len += C2IL::GetSize();
	}
	return ++len;
}

void F_C2IL::Insert(int xcoo, int ycoo)
{
	C2IL* cont = new C2IL();
	cont->m_ycoo = ycoo;
	cont->m_xcoo = xcoo;
	m_arr.push_back(cont);
}
