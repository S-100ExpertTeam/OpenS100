#pragma once

class CodeWithNumericCode
{
public:
	CodeWithNumericCode();
	CodeWithNumericCode(CString& code, int numericCode);

public:
	/*
	* Code
	* Format		: A
	* Description	: The code as defined in the feature catalogue
	*/
	CString m_code;

	/*
	* Code
	* Format		: b12
	* Description	: The code used within the subfield which using numeric code value
	*/
	int m_nmcd = 0;

public:
	static int GetSize();

	int GetNumericCode();
	void SetNumericCode(int value);

	std::wstring GetCode();
	std::string getCodeAsString();
	void SetCode(std::wstring value);
};