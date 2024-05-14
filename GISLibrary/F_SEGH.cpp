#include "stdafx.h"
#include "F_SEGH.h"
#include "NonPrintableCharacter.h"

F_SEGH::F_SEGH(void)
{
}

F_SEGH::F_SEGH(const F_SEGH& other) : Field(other)
{
	m_intp = other.m_intp;
	m_circ = other.m_circ;
	m_ycoo = other.m_ycoo;
	m_xcoo = other.m_xcoo;
	m_dist = other.m_dist;
	m_disu = other.m_disu;
	m_sbrg = other.m_sbrg;
	m_angl = other.m_angl;
}

F_SEGH::~F_SEGH(void)
{

}

F_SEGH F_SEGH::operator=(const F_SEGH& other)
{
	m_intp = other.m_intp;
	m_circ = other.m_circ;
	m_ycoo = other.m_ycoo;
	m_xcoo = other.m_xcoo;
	m_dist = other.m_dist;
	m_disu = other.m_disu;
	m_sbrg = other.m_sbrg;
	m_angl = other.m_angl;

	return *this;
}

void F_SEGH::ReadField(BYTE *&buf)
{
	m_intp = *(buf++);
}

bool F_SEGH::WriteField(CFile* file)
{
	file->Write(&m_intp, 1);

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

int F_SEGH::GetFieldLength()
{
	int len = 0;
	len += 1;
	return ++len;
}


