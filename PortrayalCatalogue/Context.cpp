#include "stdafx.h"
#include "Context.h"
#include "ContextParameter.h"

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

	void Context::GetContents(pugi::xml_node& node)
	{
		for (auto instruction = node.first_child(); instruction; instruction= instruction.next_sibling())
		{
			auto instructionName = instruction.name();
			if (!strcmp(instructionName,"parameter")) 
			{
				//std::vector<ContextParameter*> parameter;
				ContextParameter* context = new ContextParameter();
				context->GetContents(instruction);
				parameter.push_back(context);

			}
		}
	}
}