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

	reference = node.attribute("reference").as_string();
}

void S100_LineStyleReference::SetReference(const std::string& value)  { reference = value; }
void S100_LineStyleReference::SetReference(const std::wstring& value) { reference = toUtf8(value); }
std::string  S100_LineStyleReference::GetReference()  { return reference; }
std::wstring S100_LineStyleReference::GetReferenceW() { return toWide(reference); }
