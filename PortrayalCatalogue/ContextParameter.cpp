#include "stdafx.h"
#include "ContextParameter.h"

namespace Portrayal
{
	ContextParameter::ContextParameter()
	{

	}

	ContextParameter::~ContextParameter()
	{

	}

	ParameterType* ContextParameter::GetType()
	{
		return &type; 
	}

	void ContextParameter::SetType(ParameterType* value)
	{
		type = *value;
	}

	std::wstring ContextParameter::GetDefault() 
	{
		return default;
	}

	void ContextParameter::SetDefault(std::wstring& value)
	{
		default = value;
	}
}