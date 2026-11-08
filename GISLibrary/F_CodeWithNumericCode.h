#pragma once
#include "BinaryWriter.h"

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
	F_CodeWithNumericCode(const F_CodeWithNumericCode& other);
	virtual ~F_CodeWithNumericCode();

public:
	std::list<CodeWithNumericCode*> listCodeWithNumericCode;
	std::unordered_map<int, CodeWithNumericCode*> m_arr;
	std::unordered_map<std::wstring, CodeWithNumericCode*> m_arrFindForCode;

public:
	virtual F_CodeWithNumericCode operator=(const F_CodeWithNumericCode& other);

public:
	void ReadField(BYTE *&buf);
	void ReadField(BYTE *&buf, int loopCnt);
	bool WriteField(s100::io::BinaryWriter* file);
	int GetFieldLength();
	int GetCount();
	void InsertCodeNumericCode(CodeWithNumericCode* value);
	void InsertCodeNumericCode(const std::wstring& code, int numericCode);
	std::wstring GetFeatureCode(int numericCode);
	int GetCode(const std::wstring& code);
	bool HasCode(const std::wstring& code) const;

	int GetNewNumericCode();
	int AddNewCodeNumericCode(const std::wstring& code);
	int GetNumericCode(const std::wstring& code);

	CodeWithNumericCode* getCodeWithNumericCode(int numericCode);
	CodeWithNumericCode* getCodeWIthNumericCode(const std::wstring& code);
	void setCodeNumericCode(const std::wstring& code, int numericCode);
};
