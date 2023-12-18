#include "stdafx.h"
#include "F_INAS.h"
#include "ATTR.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_INAS::F_INAS()
{
}

F_INAS::F_INAS(const F_INAS& other)
{
	m_name = other.m_name;
	m_niac = other.m_niac;
	m_narc = other.m_narc;
	m_iuin = other.m_iuin;

	auto cnt = other.getATTRCount();

	for (int i = 0; i < cnt; i++) {
		auto item = new ATTR(*other.getATTR(i));
		addATTR(item);
	}
}

F_INAS::~F_INAS()
{
	for (auto i = m_arr.begin(); i != m_arr.end(); i++)
	{
		delete (*i);
	}
}

int F_INAS::GetSize()
{
	return 10;
}

void F_INAS::ReadField(BYTE *&buf)
{
	m_name.RCNM = *(buf++);
	m_name.RCID = buf2uint(buf, 4);
	m_niac = buf2uint(buf, 2);
	m_narc = buf2uint(buf, 2);
	m_iuin = *(buf++);
	while (*buf != 0x1E)
	{
		ATTR *attr = new ATTR();
		attr->m_natc = buf2uint(buf, 2);
		attr->m_atix = buf2uint(buf, 2);
		attr->m_paix = buf2uint(buf, 2);
		attr->m_atin = *(buf++);
		buf2charArr(attr->m_atvl, buf);
		m_arr.push_back(attr);
	}
}

void F_INAS::ReadField(BYTE *&buf, int loopCnt)
{
	m_name.RCNM = *(buf++);
	m_name.RCID = buf2uint(buf, 4);
	m_niac = buf2uint(buf, 2);
	m_narc = buf2uint(buf, 2);
	m_iuin = *(buf++);
	for (int i = 0; i < loopCnt; i++)
	{
		ATTR *attr = new ATTR();
		attr->m_natc = buf2uint(buf, 2);
		attr->m_atix = buf2uint(buf, 2);
		attr->m_paix = buf2uint(buf, 2);
		attr->m_atin = *(buf++);
		buf2charArr(attr->m_atvl, buf);
		m_arr.push_back(attr);
	}
}

bool F_INAS::WriteField(CFile* file)
{
	file->Write(&m_name.RCNM, 1);
	file->Write(&m_name.RCID, 4);
	file->Write(&m_niac, 2);
	file->Write(&m_narc, 2);
	file->Write(&m_iuin, 1);

	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		ATTR* attr = *itor;

		file->Write(&attr->m_natc, 2);
		file->Write(&attr->m_atix, 2);
		file->Write(&attr->m_paix, 2);
		file->Write(&attr->m_atin, 1);
		CT2CA outputString(attr->m_atvl, CP_UTF8);
		file->Write(outputString, (UINT)::strlen(outputString));
		file->Write(&NonPrintableCharacter::unitTerminator, 1);
	}
	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
}

int F_INAS::GetFieldLength()
{
	int len = 0;
	len += F_INAS::GetSize();

	for (auto i = m_arr.begin(); i != m_arr.end(); i++)
	{
		auto attr = *i;
		len += ATTR::GetOffsetToATVL();
		CT2CA outputString(attr->m_atvl, CP_UTF8);
		len += (int)::strlen(outputString) + 1;
	}

	return ++len;
}

std::vector<ATTR*> F_INAS::GetAllAttributes()
{
	return m_arr;
}

int F_INAS::getATTRCount() const
{
	return (int)m_arr.size();
}

ATTR* F_INAS::getATTR(int index) const
{
	if (index < 0 || index >= getATTRCount()) {
		return nullptr;
	}

	return m_arr.at(index);
}

void F_INAS::addATTR(ATTR* value)
{
	m_arr.push_back(value);
}

F_INAS* F_INAS::Clone() const
{
	F_INAS* f_inas = new F_INAS();
	f_inas->m_name = m_name;
	f_inas->m_niac = m_niac;
	f_inas->m_narc = m_narc;
	f_inas->m_iuin = m_iuin;
	for (const auto& iter : m_arr)
	{
		ATTR* attr = new ATTR(iter->m_natc, iter->m_atix, iter->m_paix, iter->m_atin, iter->m_atvl);
		f_inas->m_arr.push_back(attr);
	}

	return f_inas;
}
