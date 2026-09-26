#pragma once
#include <string>

class S100_Include
{
public:
	S100_Include();
	virtual ~S100_Include();

private:
	std::string href;

public:
	void SetHerf(const std::string& value);
	void SetHerf(const std::wstring& value);
	std::string  GetHerf();
	std::wstring GetHerfW();
};
