#include "stdafx.h"
#include "S100_LineStyleReference.h"

S100_LineStyleReference::S100_LineStyleReference()
{
	SetType(2);
}

S100_LineStyleReference::~S100_LineStyleReference()
{

}

void S100_LineStyleReference::GetContents(pugi::xml_node& node)
{
	if (!node) 
	{
		return;
	}

	reference = pugi::as_wide(node.attribute("reference").as_string());
}

void S100_LineStyleReference::SetReference(std::wstring& value)
{
	reference = value;
}
std::wstring S100_LineStyleReference::GetReference() 
{
	return reference;
}