#include "stdafx.h"
#include "Context.h"

namespace Portrayal
{
	Context::Context()
	{

	}

	Context::~Context()
	{

	}

	int Context::GetCountOfParameter()
	{
		return (int)parameter.size();
	}

	ContextParameter* Context::GetContextParameter(int index)
	{
		return parameter.at(index);
	}

	std::vector<ContextParameter*> Context::GetContextParameter()
	{
		return parameter;
	}

	void Context::SetContextParameter(ContextParameter* value)
	{
		parameter.push_back(value);
	}

	void Context::SetContextParameter(std::vector<ContextParameter*> value)
	{
		parameter=value;
	}
}