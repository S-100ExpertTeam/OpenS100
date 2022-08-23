#pragma once

#include "Field.h"

#include <list>
#include <unordered_map>

/*
* Matching Table with feature catalogue and numeric code.
*/
class CodeWithNumericCode;
class F_CodeWithNumericCode : public Field
{ 
public:
	F_CodeWithNumericCode();
	virtual ~F_CodeWithNumericCode();

public:
	std::list<CodeWithNumericCode*> listCodeWithNumericCode;
	std::unordered_map<int, CodeWithNumericCode*> m_arr;
	std::unordered_map<std::wstring, CodeWithNumericCode*> m_arrFindForCode;

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	bool WriteField(CFile* file);
	int GetFieldLength();
	int GetCount();
	void InsertCodeNumericCode(CodeWithNumericCode* value);
	void InsertCodeNumericCode(CString& code, int numericCode);
	CString GetFeatureCode(int numericCode);
	int GetCode(CString nc);
	bool HasCode(std::wstring& code);

	int GetNewNumericCode();
	int AddNewCodeNumericCode(CString& code);
	int AddNewCodeNumericCode(std::wstring& code);
	int GetNumericCode(CString& code);
	int GetNumericCode(std::wstring& code);
};