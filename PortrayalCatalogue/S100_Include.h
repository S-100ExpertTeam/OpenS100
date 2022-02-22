#pragma once
#include <string>

class S100_Include
{
public:
	S100_Include();
	virtual ~S100_Include();

private:
	std::wstring href;

public:
	void SetHerf(std::wstring& value);
	std::wstring GetHerf();
}; 