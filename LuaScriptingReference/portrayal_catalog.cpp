#include "stdafx.h"
#include "portrayal_catalog.h"

std::vector<context_parameter> pc_get_context_parameters_from_pc(PortrayalCatalogue* pc)
{
	std::vector<context_parameter> result;

	auto context = pc->GetContext();
	auto cnt = context->GetCountOfParameter();

	for (int i = 0; i < cnt; i++)
	{
		auto cp = context->GetContextParameter(i);
		if (cp)
		{
			auto id = cp->GetIdAsString();
			auto enumType = cp->GetType();
			std::string strType = "";
			auto default_value = cp->GetDefaultAsString();

			if (enumType == ParameterType::Boolean)
			{
				strType = "boolean";
			}
			else if (enumType == ParameterType::Integer)
			{
				strType = "integer";
			}
			else if (enumType == ParameterType::Double)
			{
				strType = "real";
			}
			else if (enumType == ParameterType::String)
			{
				strType = "text";
			}
			else if (enumType == ParameterType::Date)
			{
				strType = "date";
			}

			result.push_back({ id, strType, default_value });
		}
	}

	return result;
}