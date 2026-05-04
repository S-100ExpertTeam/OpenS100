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

	ParameterType ContextParameter::GetType()
	{
		return type;
	}

	void ContextParameter::SetType(ParameterType val)
	{
		type = val;
	}

	void ContextParameter::SetType(const std::string& val)
	{
		if (val == "Boolean")       type = ParameterType::Boolean;
		else if (val == "Integer")  type = ParameterType::Integer;
		else if (val == "Double")   type = ParameterType::Double;
		else if (val == "String")   type = ParameterType::String;
		else if (val == "Date")     type = ParameterType::Date;
	}

	void ContextParameter::SetType(const std::wstring& val)
	{
		SetType(toUtf8(val));
	}

	std::string  ContextParameter::GetDefault()  { return defaultValue; }
	std::wstring ContextParameter::GetDefaultW() { return toWide(defaultValue); }

	void ContextParameter::SetDefault(const std::string& val)
	{
		defaultValue = val;
		setValue(val);
	}

	void ContextParameter::SetDefault(const std::wstring& val)
	{
		SetDefault(toUtf8(val));
	}

	std::string  ContextParameter::getValue()  { return value; }
	std::wstring ContextParameter::getValueW() { return toWide(value); }

	void ContextParameter::setValue(const std::string& val)
	{
		value = val;
	}

	void ContextParameter::setValue(const std::wstring& val)
	{
		value = toUtf8(val);
	}

	std::string ContextParameter::getName()
	{
		auto cntDes = GetCountOfDescription();
		if (cntDes > 0)
		{
			auto des = GetDescription(0);
			if (des)
			{
				return des->name;
			}
		}
		return std::string("");
	}

	void ContextParameter::GetContents(pugi::xml_node& node)
	{
		auto idAttri = node.attribute("id");
		if (idAttri != nullptr)
		{
			SetId(std::string(idAttri.value()));
		}

		for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
		{
			auto instructionName = instruction.name();

			if (!strcmp(instructionName, "description"))
			{
				S100_Description* des = new S100_Description();
				des->GetContents(instruction);
				AddDescription(des);
			}
			else if (!strcmp(instructionName, "type"))
			{
				SetType(std::string(instruction.child_value()));
			}
			else if (!strcmp(instructionName, "default"))
			{
				SetDefault(std::string(instruction.child_value()));
			}
		}
	}
}
