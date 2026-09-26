#pragma once
#include "S100_WithParam.h"

#include <vector>

class S100_CallTemplate
{
public:
	S100_CallTemplate();
	virtual ~S100_CallTemplate();

private:
	std::string name;
	std::vector<S100_WithParam*> params;

public:
	void SetName(const std::string& value);
	void SetName(const std::wstring& value);
	void SetParam(S100_WithParam* value);
	void SetParams(std::vector<S100_WithParam*> value);

	std::string  GetName();
	std::wstring GetNameW();
	S100_WithParam* GetParam(int index);
	std::vector<S100_WithParam*> GetParams();
};
