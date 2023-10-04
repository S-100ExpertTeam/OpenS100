#include "stdafx.h"
#include "F_FASC.h"
#include "ATTR.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_FASC::F_FASC(void)
{

}

F_FASC::F_FASC(const F_FASC& other)
{
	m_name = other.m_name;
	m_nfac = other.m_nfac;
	m_narc = other.m_narc;
	m_faui = other.m_faui;

	for (auto i = other.m_arr.begin(); i != other.m_arr.end(); i++) {
		auto item = new ATTR(**i);
		m_arr.push_back(item);
	}
}

F_FASC::~F_FASC(void)
{
	for (auto i = m_arr.begin(); i != m_arr.end(); i++) {
		delete (*i);
	}
}

void F_FASC::ReadField(BYTE *&buf)
{
	m_name.RCNM = *(buf++);
	m_name.RCID = buf2uint(buf, 4);
	m_nfac = buf2uint(buf, 2);
	m_narc = buf2uint(buf, 2);
	m_faui = *(buf++);
}

void F_FASC::ReadField(BYTE *&buf, int loopCnt)
{
	m_name.RCNM = *(buf++);
	m_name.RCID = buf2uint(buf, 4);
	m_nfac = buf2uint(buf, 2);
	m_narc = buf2uint(buf, 2);
	m_faui = *(buf++);
}

bool F_FASC::WriteField(CFile* file)
{
	file->Write(&m_name.RCNM, 1);
	file->Write(&m_name.RCID, 4);
	file->Write(&m_nfac, 2);
	file->Write(&m_narc, 2);
	file->Write(&m_faui, 1);

	for (auto i = m_arr.begin(); i != m_arr.end(); i++)
	{
		file->Write(&(*i)->m_natc, 2);
		file->Write(&(*i)->m_atix, 2);
		file->Write(&(*i)->m_paix, 2);
		file->Write(&(*i)->m_atin, 1);
		CT2CA outputString((*i)->m_atvl, CP_UTF8);
		file->Write(outputString, (UINT)::strlen(outputString));
		file->Write(&NonPrintableCharacter::unitTerminator, 1);
	}

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}


int F_FASC::GetFieldLength()
{
	int len = 0;
	len += 10;
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		ATTR *cont = *itor;
		len += ATTR::GetOffsetToATVL();
		CT2CA outputString(cont->m_atvl, CP_UTF8);
		len += (int)::strlen(outputString) + 1;
	}
	return ++len;
}

int F_FASC::getATTRCount() const
{
	return m_arr.size();
}

ATTR* F_FASC::getATTR(int index) const
{
	if (index < 0 || index >= getATTRCount()) {
		return nullptr;
	}

	return m_arr.at(index);
}

void F_FASC::addATTR(ATTR* value)
{
	m_arr.push_back(value);
}