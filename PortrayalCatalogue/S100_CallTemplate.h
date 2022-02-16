#pragma once
#include "S100_WithParam.h"

#include <vector>

class S100_CallTemplate
{
public:
	S100_CallTemplate();
	virtual ~S100_CallTemplate();

private:
	std::wstring name;
	std::vector<S100_WithParam*> params;

public:
	void SetName(std::wstring& value);
	void SetParam(S100_WithParam* value);
	void SetParams(std::vector<S100_WithParam*> value);

	std::wstring GetName();
	S100_WithParam* GetParam(int index);
	std::vector<S100_WithParam*> GetParams();
};