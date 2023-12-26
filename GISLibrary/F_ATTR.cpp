#include "stdafx.h"
#include "F_ATTR.h"
#include "ATTR.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_ATTR::F_ATTR()
{
}

F_ATTR::F_ATTR(const F_ATTR& other)
{
	for (const auto& iter : other.m_arr)
	{
		ATTR* attr = new ATTR(iter->m_natc, iter->m_atix, iter->m_paix, iter->m_atin, iter->m_atvl);
		m_arr.push_back(attr);
	}

}

F_ATTR::~F_ATTR()
{
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		delete* itor;
	}
}

void F_ATTR::ReadField(BYTE *&buf)
{
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

void F_ATTR::ReadField(BYTE *&buf, int loopCnt)
{
	for(int i = 0; i < loopCnt; i++)
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

bool F_ATTR::WriteField(CFile* file)
{
	for (auto i = m_arr.begin(); i != m_arr.end(); i++)
	{
		ATTR* attr = *i;

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

int F_ATTR::GetFieldLength()
{
	unsigned len = 0;

	for (auto i = m_arr.begin(); i != m_arr.end(); i++)
	{
		auto attr = *i;

		len += ATTR::GetOffsetToATVL();

		CT2CA outputString(attr->m_atvl, CP_UTF8);

		len += (unsigned int)::strlen(outputString) + 1;
	}
	return ++len;
}

void F_ATTR::Insert(ATTR* attr)
{
	m_arr.push_back(attr);
}

void F_ATTR::Insert(int natc, int atix, int paix, int atin, CString atvl)
{
	ATTR* attr = new ATTR(natc, atix, paix, atin, atvl);
	m_arr.push_back(attr);
}

int F_ATTR::getCount() const
{
	return (int)m_arr.size();
}

ATTR* F_ATTR::getATTR(int index) const
{
	if (index < 0 || index >= (int)m_arr.size()) {
		return nullptr;
	}

	return m_arr.at(index);
}

F_ATTR* F_ATTR::Clone() const
{
	F_ATTR* f_attr = new F_ATTR();
	for (const auto& iter : m_arr)
	{
		ATTR* attr = new ATTR(iter->m_natc, iter->m_atix, iter->m_paix, iter->m_atin, iter->m_atvl);
		f_attr->m_arr.push_back(attr);
	}

	return f_attr;
}

