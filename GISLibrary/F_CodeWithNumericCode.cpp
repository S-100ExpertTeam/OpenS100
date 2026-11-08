#include "stdafx.h"
#include "F_CodeWithNumericCode.h"
#include "CodeWithNumericCode.h"
#include "ISO8211Fuc.h"
#include "NonPrintableCharacter.h"

F_CodeWithNumericCode::F_CodeWithNumericCode()
{

}

F_CodeWithNumericCode::F_CodeWithNumericCode(const F_CodeWithNumericCode& other) : Field(other)
{
	for (const auto& iter : other.listCodeWithNumericCode)
	{
		auto value = new CodeWithNumericCode(*iter);
		listCodeWithNumericCode.push_back(value);
		m_arr.insert({ value->GetNumericCode(), value });
		m_arrFindForCode.insert({ value->GetCode(), value });
	}
}

F_CodeWithNumericCode::~F_CodeWithNumericCode()
{
	for (auto itor = m_arr.begin(); itor != m_arr.end(); itor++)
	{
		CodeWithNumericCode* attr = itor->second;
		delete attr;
	}
}

F_CodeWithNumericCode F_CodeWithNumericCode::operator=(const F_CodeWithNumericCode& other)
{
	for (auto& iter : listCodeWithNumericCode)
	{
		if (iter)
		{
			delete iter;
			iter = nullptr;
		}
	}
	listCodeWithNumericCode.clear();
	m_arr.clear();
	m_arrFindForCode.clear();

	for (const auto& iter : other.listCodeWithNumericCode)
	{
		auto value = new CodeWithNumericCode(*iter);
		listCodeWithNumericCode.push_back(value);
		m_arr.insert({ value->GetNumericCode(), value });
		m_arrFindForCode.insert({ value->GetCode(), value });
	}

	return *this;
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

bool F_CodeWithNumericCode::WriteField(s100::io::BinaryWriter* file)
{
	for (auto i = listCodeWithNumericCode.begin(); i != listCodeWithNumericCode.end(); i++)
	{
		s100::iso8211::WriteUtf8Unit(file, (*i)->m_code);
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
		len += static_cast<int>(s100::iso8211::Utf8Length(cnc->m_code)) + 1;
		len += CodeWithNumericCode::GetSize();
	}
	return ++len;
}

std::wstring F_CodeWithNumericCode::GetFeatureCode(int numericCode)
{
	auto aitor = m_arr.find(numericCode);
	if (aitor != m_arr.end())
	{
		CodeWithNumericCode* nc = aitor->second;
		return nc->m_code;
	}
	return L"";
}

int F_CodeWithNumericCode::GetCode(const std::wstring& code)
{
	auto aitor = m_arrFindForCode.find(code);
	if (aitor!=m_arrFindForCode.end()) 
	{
		CodeWithNumericCode* nc = aitor->second;
		return nc->GetNumericCode();
	}
	return 0;
}

bool F_CodeWithNumericCode::HasCode(const std::wstring& code) const
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

void F_CodeWithNumericCode::InsertCodeNumericCode(const std::wstring& code, int numericCode)
{
	auto codeWithNumericCode = new CodeWithNumericCode(code, numericCode);
	InsertCodeNumericCode(codeWithNumericCode);
}

int F_CodeWithNumericCode::GetNewNumericCode()
{
	int limitNumericCode = 65535;
	for (int i = 1; i <= limitNumericCode; i++)
	{
		if (m_arr.find(i) == m_arr.end())
		{
			return i;
		}
	}

	return -1;
}

int F_CodeWithNumericCode::AddNewCodeNumericCode(const std::wstring& code)
{
	int numericCode = GetNewNumericCode();
	auto codeWithNumericCode = new CodeWithNumericCode(code, numericCode);
	InsertCodeNumericCode(codeWithNumericCode);
	return numericCode;
}

int F_CodeWithNumericCode::GetNumericCode(const std::wstring& code)
{
	bool find = HasCode(code);
	if (find)
	{
		auto numericCode = GetCode(code);
		return numericCode;
	}

	int newNumericCode = AddNewCodeNumericCode(code);
	return newNumericCode;
}

CodeWithNumericCode* F_CodeWithNumericCode::getCodeWithNumericCode(int numericCode)
{
	auto item = m_arr.find(numericCode);
	if (item != m_arr.end()) {
		return item->second;
	}

	return nullptr;
}

CodeWithNumericCode* F_CodeWithNumericCode::getCodeWIthNumericCode(const std::wstring& code)
{
	auto item = m_arrFindForCode.find(code);
	if (item != m_arrFindForCode.end()) {
		return item->second;
	}

	return nullptr;
}

void F_CodeWithNumericCode::setCodeNumericCode(const std::wstring& code, int numericCode)
{
	auto item = getCodeWIthNumericCode(code);

	if (!item) { // new
		InsertCodeNumericCode(code, numericCode);
	}
	else { // update
		auto oldNumericCode = item->GetNumericCode();
		item->SetNumericCode(numericCode);
		auto find = m_arr.find(oldNumericCode);
		if (find != m_arr.end()) {
			m_arr.erase(find);
			m_arr.insert({ numericCode, item });
		}
	}
}

