#include "stdafx.h"
#include "StringUtil.h"
#include "DefinitionSources.h"

DefinitionSources::DefinitionSources()
{

}

DefinitionSources::~DefinitionSources()
{
	for (auto i = definitionSource.begin(); i != definitionSource.end(); i++)
	{
		delete i->second;
	}
}

void DefinitionSources::GetContents(pugi::xml_node& /*node*/)
{
}

void DefinitionSources::SetSourceIdentifier(std::string _value)
{
	sourceIdentifier = _value;
}

std::string& DefinitionSources::GetSourceIdentifier()
{
	return sourceIdentifier;
}

void DefinitionSources::SetDefinitionSource(std::unordered_map<std::string, DefinitionSource*> _value)
{
	definitionSource = _value;
}

std::unordered_map<std::string, DefinitionSource*>& DefinitionSources::GetDefinitionSource()
{
	return definitionSource;
}

bool DefinitionSources::InsertDefinitionSource(DefinitionSource* /*value*/)
{
	//if (definitionSource.find(value->))

	//definitionSource[definition->Getvalue()] = definition;
	return false;
}

void DefinitionSources::SetSourceIdentifier(const char* _value)
{
	std::string s(_value ? _value : "");
	SetSourceIdentifier(s);
}

void DefinitionSources::SetSourceIdentifier(std::wstring _value)
{
	std::string s = toUtf8(_value);
	SetSourceIdentifier(s);
}

std::wstring DefinitionSources::GetSourceIdentifierW()
{
	return toWide(GetSourceIdentifier());
}
