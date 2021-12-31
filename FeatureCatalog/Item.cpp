#include "stdafx.h"
#include "Item.h"

Item::Item()
{

}

Item::Item(const Item& item) : Item()
{
	if (nullptr != item.remarks)
	{
		SetRemarks(*item.remarks);
	}
}

Item::~Item()
{
	delete remarks;
	remarks = nullptr;
}

void Item::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "S100FC:name"))
		{
			name = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100FC:definition"))
		{
			definition = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100FC:code"))
		{
			std::string value = instruction.child_value();
			SetCode(value);
		}
		else if (!strcmp(instructionName, "S100FC:remarks"))
		{
			std::string value = instruction.child_value();
			SetRemarks(value);
		}
		else if (!strcmp(instructionName, "S100FC:alias"))
		{
			alias.push_back(pugi::as_wide(instruction.child_value()));
		}
		else if (!strcmp(instructionName, "S100FC:definitionReference"))
		{
			definitionReference.GetContents(instruction);
		}
	}
}

bool Item::CompareCode(std::string& value)
{
	std::wstring wval = pugi::as_wide(value);
	return CompareCode(wval);
}

bool Item::CompareCode(std::wstring& value)
{
	if (code.compare(value) == 0)
	{
		return true;
	}
	return false;
}

const std::wstring& Item::GetName()
{
	return name;
}

void Item::SetName(std::wstring& value)
{
	name = value;
}

const std::wstring& Item::GetDefinition()
{
	return definition;
}

void Item::SetDefinition(std::wstring& value)
{
	definition = value;
}

const std::string Item::GetCode()
{
	return pugi::as_utf8(code);
}

const std::wstring Item::GetCodeAsWString()
{
	return code;
}

void Item::SetCode(std::wstring& value)
{
	code = value;
}

void Item::SetCode(std::string& value)
{
	code = pugi::as_wide(value);
}

void Item::NullCheckRemarks()
{
	if (nullptr == remarks)
	{
		remarks = new std::wstring();
	}
}

void Item::SetRemarks(std::string& value)
{
	NullCheckRemarks();
	*remarks = pugi::as_wide(value);
}

void Item::SetRemarks(std::wstring& value)
{
	NullCheckRemarks();
	*remarks = value;
}

const std::string Item::GetRemarks()
{
	if (nullptr != remarks)
	{
		return pugi::as_utf8(*remarks);
	}
	return "";
}

const std::wstring Item::GetRemarksAsWString()
{
	if (nullptr != remarks)
	{
		return *remarks;
	}
	return L"";
}

const bool Item::IsEmptyRemarks()
{
	if (nullptr != remarks)
	{
		return remarks->empty();
	}
	return true;
}

std::list<std::wstring>& Item::GetAliasPointer()
{
	return alias;
}

Item& Item::operator = (const Item& item)
{
	this->name = item.name;
	this->definition = item.definition;
	this->code = item.code;
	this->alias = item.alias;
	this->definitionReference = item.definitionReference;

	if (nullptr != item.remarks)
	{
		NullCheckRemarks();

		*this->remarks = *item.remarks;
	}
	return *this;
}