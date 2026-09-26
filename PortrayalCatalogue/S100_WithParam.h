#pragma once
#include <string>

class S100_WithParam
{
public:
	S100_WithParam();
	virtual ~S100_WithParam();

private:
	std::string name;
	std::string select;
	std::string value;

public:
	void SetName(const std::string& value);
	void SetName(const std::wstring& value);
	void SetSelect(const std::string& value);
	void SetSelect(const std::wstring& value);
	void SetValue(const std::string& _value);
	void SetValue(const std::wstring& _value);

	std::string  GetName();
	std::wstring GetNameW();
	std::string  GetSelect();
	std::wstring GetSelectW();
	std::string  GetValue();
	std::wstring GetValueW();
};
