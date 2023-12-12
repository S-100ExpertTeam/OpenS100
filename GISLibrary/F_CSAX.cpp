#include "stdafx.h"
#include "F_CSAX.h"
#include "CSAX.h"
#include "NonPrintableCharacter.h"

F_CSAX::F_CSAX(void)
{

}

F_CSAX::F_CSAX(const F_CSAX& other)
{
	for (const auto& iter : other.m_arr)
		Insert(iter->m_axty, iter->m_axum);
}

F_CSAX::~F_CSAX(void)
{
	for (auto i = m_arr.begin(); i != m_arr.end(); i++)
	{
		delete* i;
	}
}

void F_CSAX::ReadField(BYTE *&buf)
{
	while(*buf != 0x1E) 
	{
		CSAX *csax = new CSAX();
		csax->m_axty = *(buf++);
		csax->m_axum = *(buf++);	
		m_arr.push_back(csax);
	}
}

void F_CSAX::ReadField(BYTE *&buf, int loopCnt)
{
	for(int i = 0;i < loopCnt; i++)
	{
		CSAX *csax = new CSAX();

		csax->m_axty = *(buf++);
		csax->m_axum = *(buf++);
			
		m_arr.push_back(csax);
	}
}

bool F_CSAX::WriteField(CFile* file)
{
	for (auto i = m_arr.begin(); i != m_arr.end(); i++)
	{
		file->Write(&(*i)->m_axty, 1);
		file->Write(&(*i)->m_axum, 1);
	}

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

int F_CSAX::GetFieldLength()
{
	int len = 0;

	for (auto i = m_arr.begin(); i != m_arr.end(); i++)
	{
		len += 1;
		len += 1;
	}

	return ++len;
}

void F_CSAX::Insert(int axty, int axum)
{
	CSAX* csax = new CSAX();
	csax->m_axty = axty;
	csax->m_axum = axum;
	m_arr.push_back(csax);
}

