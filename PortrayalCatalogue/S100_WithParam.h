  #pragma once
#include <string>

class S100_WithParam
{
public:
	S100_WithParam();
	virtual ~S100_WithParam();

private:
	std::wstring name;
	std::wstring select;
	std::wstring value;

public:
	void SetName(std::wstring& value);
	void SetSelect(std::wstring& value);
	void SetValue(std::wstring& _value);

	std::wstring GetName();
	std::wstring GetSelect();
	std::wstring GetValue();
};