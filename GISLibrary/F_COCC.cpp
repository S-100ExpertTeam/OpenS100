#include "stdafx.h"
#include "F_COCC.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_COCC::F_COCC()
{
	m_coui = 1;
	m_coix = 0;
	m_ncor = 0;
}

F_COCC::F_COCC(const F_COCC& other)
{
	m_coui = other.m_coui;
	m_coix = other.m_coix;
	m_ncor = other.m_ncor;
}

F_COCC::~F_COCC()
{

}

void F_COCC::ReadField(BYTE*& buf)
{
	m_coui = buf2uint(buf, 1);
	m_coix = buf2uint(buf, 2);
	m_ncor = buf2uint(buf, 2);
}

int F_COCC::GetFieldLength()
{
	int len = 0;
	len += 1;
	len += 2;
	len += 2;
	return ++len;
}

F_COCC* F_COCC::Clone() const
{
	F_COCC* f_cocc = new F_COCC();
	f_cocc->m_coui = m_coui;
	f_cocc->m_coix = m_coix;
	f_cocc->m_ncor = m_ncor;

	return f_cocc;
}
