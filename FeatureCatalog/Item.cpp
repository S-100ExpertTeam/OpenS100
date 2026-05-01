#include "StringUtil.h"
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

	delete definitionReference;
	definitionReference = nullptr;
}

void Item::GetContents(pugi::xml_node& node)
{
	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const char* instructionName = instruction.name();

		if (!strcmp(instructionName, "S100FC:name"))
		{
			name = instruction.child_value();
		}
		else if (!strcmp(instructionName, "S100FC:definition"))
		{
			definition = instruction.child_value();
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
			alias.push_back(instruction.child_value());
		}
		else if (!strcmp(instructionName, "S100FC:definitionReference"))
		{
			if (nullptr == definitionReference)
			{
				definitionReference = new DefinitionReference();
			}

			definitionReference->GetContents(instruction);
		}
	}
}



bool Item::CompareCode(std::string& value)
{
	if (code.compare(value) == 0)
	{
		return true;
	}
	return false;
}

const std::string& Item::GetName()
{
	return name;
}

void Item::SetName(std::string& value)
{
	name = value;
}

const std::string& Item::GetDefinition()
{
	return definition;
}

void Item::SetDefinition(std::string& value)
{
	definition = value;
}

const std::string Item::GetCode()
{
	return code;
}

std::wstring Item::GetCodeAsWString()
{
	return toWide(code);
}

void Item::SetCode(std::string& value)
{
	code = value;
}


void Item::NullCheckRemarks()
{
	if (nullptr == remarks)
	{
		remarks = new std::string();
	}
}

void Item::SetRemarks(std::string& value)
{
	NullCheckRemarks();
	*remarks = value;
}


const std::string Item::GetRemarks()
{
	if (nullptr != remarks)
	{
		return *remarks;
	}
	return "";
}

std::wstring Item::GetRemarksAsWString()
{
	if (remarks) {
		return toWide(*remarks);
	}

	return "";
}

const bool Item::IsEmptyRemarks()
{
	if (nullptr != remarks)
	{
		return remarks->empty();
	}
	return true;
}

const std::list<std::string>& Item::GetAlias()
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

void Item::setSourceIdentifier(std::string value)
{
	if (!definitionReference)
	{
		definitionReference = new DefinitionReference();
	}

	definitionReference->SetSourceIdentifier(value);
}

std::wstring Item::getSourceIdentifierAsWString()
{
	if (definitionReference)
	{
		auto si = definitionReference->GetSourceIdentifier();
		if (si)
		{
			return toWide(*si);
		}
	}

	return "";
}

std::string Item::getSourceIdentifier()
{
	return getSourceIdentifierAsWString();
}