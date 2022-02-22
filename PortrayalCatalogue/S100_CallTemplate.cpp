#include "stdafx.h"
#include "S100_CallTemplate.h"

S100_CallTemplate::S100_CallTemplate()
{

}

S100_CallTemplate::~S100_CallTemplate()
{
	for (auto itor = params.begin(); itor != params.end(); itor++)
		delete *itor;
}

void S100_CallTemplate::SetName(std::wstring& value)
{
	name = value;
}

void S100_CallTemplate::SetParam(S100_WithParam* value) 
{
	params.push_back(value);
}

void S100_CallTemplate::SetParams(std::vector<S100_WithParam*> value) 
{
	params = value;
}

std::wstring S100_CallTemplate::GetName() 
{
	return name;
}

S100_WithParam* S100_CallTemplate::GetParam(int index) 
{
	return params.at(index);
}

std::vector<S100_WithParam*> S100_CallTemplate::GetParams() 
{
	return params;
}