#include "stdafx.h"
#include "F_CodeWithNumericCode.h"
#include "CodeWithNumericCode.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_CodeWithNumericCode::F_CodeWithNumericCode()
{

}

F_CodeWithNumericCode::~F_CodeWithNumericCode()
{
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		CodeWithNumericCode* attr = itor->second;
		delete attr;
	}
}

void F_CodeWithNumericCode::ReadField(BYTE *&buf)
{
	while (*buf != 0x1E)
	{
		CodeWithNumericCode *cnc = new CodeWithNumericCode();
		buf2charArr(cnc->m_code, buf);
		cnc->m_nmcd = buf2uint(buf, 2);
		m_arr.insert({ cnc->m_nmcd, cnc });
		m_arrFindForCode.insert({ std::wstring(cnc->m_code), cnc });
	}
}

void F_CodeWithNumericCode::ReadField(BYTE *&buf, int loopCnt)
{
	for (int i = 0; i < loopCnt; i++)
	{
		CodeWithNumericCode *cnc = new CodeWithNumericCode();
		buf2charArr(cnc->m_code, buf);
		cnc->m_nmcd = buf2uint(buf, 2);
		m_arr.insert({ cnc->m_nmcd, cnc });
		m_arrFindForCode.insert({ std::wstring(cnc->m_code), cnc });
	}
}

BOOL F_CodeWithNumericCode::Save(CFile *file)
{
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		CodeWithNumericCode *cnc = itor->second;
		CT2CA outputString(cnc->m_code, CP_UTF8);
		file->Write(outputString, (UINT)::strlen(outputString));
		file->Write(&NonPrintableCharacter::unitTerminator, 1);
		file->Write(&cnc->m_nmcd, 2);
	}
	file->Write(&NonPrintableCharacter::fieldTerminator, 1);
	return TRUE;
}

int F_CodeWithNumericCode::GetFieldLength()
{
	unsigned len = 0;
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		CodeWithNumericCode *cnc = itor->second;
		CT2CA outputString(cnc->m_code, CP_UTF8);
		len += (unsigned int)::strlen(outputString) + 1;
		len += CodeWithNumericCode::GetSize();
	}
	return ++len;
}

CString F_CodeWithNumericCode::GetFeatureCode(int numericCode)
{
	auto aitor = m_arr.find(numericCode);
	if (aitor != m_arr.end())
	{
		CodeWithNumericCode* nc = aitor->second;
		return nc->m_code;
	}
	return L"";
}

int F_CodeWithNumericCode::GetCode(CString numericCode)
{
	auto aitor = m_arrFindForCode.find(std::wstring(numericCode));
	if (aitor!=m_arrFindForCode.end()) 
	{
		CodeWithNumericCode* nc = aitor->second;
		return nc->GetNumericCode();
	}
	return 0;
}

int F_CodeWithNumericCode::GetCount()
{
	return (int)m_arr.size();
}

void F_CodeWithNumericCode::InsertCodeNumericCode(CodeWithNumericCode* value)
{
	m_arr.insert({ value->GetNumericCode(), value });
	m_arrFindForCode.insert({ value->GetCode(), value });
}