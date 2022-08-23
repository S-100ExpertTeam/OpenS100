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
		//m_arr.insert({ cnc->m_nmcd, cnc });
		//m_arrFindForCode.insert({ std::wstring(cnc->m_code), cnc });

		InsertCodeNumericCode(cnc);
	}
}

void F_CodeWithNumericCode::ReadField(BYTE *&buf, int loopCnt)
{
	for (int i = 0; i < loopCnt; i++)
	{
		CodeWithNumericCode *cnc = new CodeWithNumericCode();
		buf2charArr(cnc->m_code, buf);
		cnc->m_nmcd = buf2uint(buf, 2);
		//m_arr.insert({ cnc->m_nmcd, cnc });
		//m_arrFindForCode.insert({ std::wstring(cnc->m_code), cnc });

		InsertCodeNumericCode(cnc);
	}
}

bool F_CodeWithNumericCode::WriteField(CFile* file)
{
	for (auto i = listCodeWithNumericCode.begin(); i != listCodeWithNumericCode.end(); i++)
	{
		CT2CA outputStringCode((*i)->m_code, CP_UTF8);
		file->Write(outputStringCode, (UINT)::strlen(outputStringCode));
		file->Write(&NonPrintableCharacter::unitTerminator, 1);

		file->Write(&(*i)->m_nmcd, 2);
	}

	file->Write(&NonPrintableCharacter::fieldTerminator, 1);

	return true;
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

bool F_CodeWithNumericCode::HasCode(std::wstring& code)
{
	auto item = m_arrFindForCode.find(code);
	if (item == m_arrFindForCode.end())
	{
		return false;
	}

	return true;
}

int F_CodeWithNumericCode::GetCount()
{
	return (int)m_arr.size();
}

void F_CodeWithNumericCode::InsertCodeNumericCode(CodeWithNumericCode* value)
{
	listCodeWithNumericCode.push_back(value);
	m_arr.insert({ value->GetNumericCode(), value });
	m_arrFindForCode.insert({ value->GetCode(), value });
}

void F_CodeWithNumericCode::InsertCodeNumericCode(CString& code, int numericCode)
{
	auto codeWithNumericCode = new CodeWithNumericCode(code, numericCode);
	InsertCodeNumericCode(codeWithNumericCode);
}

int F_CodeWithNumericCode::GetNewNumericCode()
{
	int limitNumericCode = 65535;
	for (int i = 0; i <= limitNumericCode; i++)
	{
		if (m_arr.find(i) == m_arr.end())
		{
			return i;
		}
	}

	return -1;
}

int F_CodeWithNumericCode::AddNewCodeNumericCode(CString& code)
{
	int numericCode = GetNewNumericCode();
	auto codeWithNumericCode = new CodeWithNumericCode(code, numericCode);
	InsertCodeNumericCode(codeWithNumericCode);
	return numericCode;
}

int F_CodeWithNumericCode::AddNewCodeNumericCode(std::wstring& code)
{
	CString strCode = code.c_str();
	return AddNewCodeNumericCode(strCode);
}

int F_CodeWithNumericCode::GetNumericCode(CString& code)
{
	std::wstring wstr(code);
	return GetNumericCode(wstr);
}

int F_CodeWithNumericCode::GetNumericCode(std::wstring& code)
{
	bool find = HasCode(code);
	if (find)
	{
		auto numericCode = GetCode(code.c_str());
		return numericCode;
	}

	int newNumericCode = AddNewCodeNumericCode(code);
	return newNumericCode;
}