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

	void ContextParameter::SetType(std::wstring value)
	{
		if (!StrCmpW(value.c_str(),L"Boolean"))
		{
			type = ParameterType::Boolean;
		}

		else if (!StrCmpW(value.c_str(), L"Integer"))
		{
			type = ParameterType::Integer;
		}

		else if (!StrCmpW(value.c_str(), L"Double"))
		{
			type = ParameterType::Double;
		}

		else if (!StrCmpW(value.c_str(), L"String"))
		{
			type = ParameterType::String;
		}

		else if (!StrCmpW(value.c_str(), L"Date"))
		{
			type = ParameterType::Date;
		}
	}

	std::wstring ContextParameter::GetDefault() 
	{
		return default;
	}

	void ContextParameter::SetDefault(std::wstring& value)
	{
		default = value;
	}

	void ContextParameter::GetContents(pugi::xml_node& node)
	{
		auto idAttri = node.attribute("id");
		if (idAttri!=nullptr)
		{
			auto idValue =pugi::as_wide( idAttri.value());
			SetId(idValue);
		}

		for (auto instruction = node.first_child(); instruction; instruction= instruction.next_sibling())
		{
			auto instructionName = instruction.name();

			if (!strcmp(instructionName,"description")) 
			{
				S100_Description* des = new S100_Description();
				des->GetContents(instruction);

				AddDescription(des);
			}
			else if (!strcmp(instructionName,"type"))
			{
				SetType(pugi::as_wide(instruction.child_value()));
			}
			else if (!strcmp(instructionName,"default"))
			{
				default = pugi::as_wide(instruction.child_value());
			}
		}
	}
}