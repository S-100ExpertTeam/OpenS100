#pragma once
#include <string>

class CodeWithNumericCode
{
public:
	CodeWithNumericCode();
	CodeWithNumericCode(const CodeWithNumericCode& other);
	CodeWithNumericCode(const std::wstring& code, int numericCode);

public:
	/*
	* Code
	* Format		: A
	* Description	: The code as defined in the feature catalogue
	*/
	std::wstring m_code;

	/*
	* Code
	* Format		: b12
	* Description	: The code used within the subfield which using numeric code value
	*/
	int m_nmcd = 0;

public:
	virtual CodeWithNumericCode operator=(const CodeWithNumericCode& other);

public:
	static int GetSize();

	int GetNumericCode();
	void SetNumericCode(int value);

	std::wstring GetCode();
	std::string getCodeAsString();
	void SetCode(std::wstring value);
};