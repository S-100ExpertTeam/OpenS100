#include "stdafx.h"
#include "S100_LineSymbol.h"

S100_LineSymbol::S100_LineSymbol()
{
	rotation = "0.0";
	scaleFactor = "1.0";
	crsType = "LocalCRS";
}

S100_LineSymbol::~S100_LineSymbol()
{

}

void S100_LineSymbol::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}
	for each (pugi::xml_attribute attri in node.attributes())
	{
		auto attriName = attri.name();
		if (!strcmp(attriName, "reference"))
		{
			reference = attri.value();
		}
		else if (!strcmp(attriName, "rotation"))
		{
			rotation = attri.value();
		}
		else if (!strcmp(attriName, "scaleFactor"))
		{
			scaleFactor = attri.value();
		}
		else if (!strcmp(attriName, "crsType"))
		{
			crsType = attri.value();
		}
	}

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "position"))
		{
			position = instruction.child_value();
		}
	}
}

void S100_LineSymbol::SetReference(const std::string& value)  { reference = value; }
void S100_LineSymbol::SetReference(const std::wstring& value) { reference = toUtf8(value); }
void S100_LineSymbol::SetRotation(const std::string& value)   { rotation = value; }
void S100_LineSymbol::SetRotation(const std::wstring& value)  { rotation = toUtf8(value); }
void S100_LineSymbol::SetScaleFactor(const std::string& value)  { scaleFactor = value; }
void S100_LineSymbol::SetScaleFactor(const std::wstring& value) { scaleFactor = toUtf8(value); }
void S100_LineSymbol::SetCrsType(const std::string& value)  { crsType = value; }
void S100_LineSymbol::SetCrsType(const std::wstring& value) { crsType = toUtf8(value); }
void S100_LineSymbol::SetPosition(const std::string& value)  { position = value; }
void S100_LineSymbol::SetPosition(const std::wstring& value) { position = toUtf8(value); }

std::string  S100_LineSymbol::GetReference()   { return reference; }
std::wstring S100_LineSymbol::GetReferenceW()  { return toWide(reference); }
std::string  S100_LineSymbol::GetRotation()    { return rotation; }
std::wstring S100_LineSymbol::GetRotationW()   { return toWide(rotation); }
std::string  S100_LineSymbol::GetScaleFactor() { return scaleFactor; }
std::wstring S100_LineSymbol::GetScaleFactorW(){ return toWide(scaleFactor); }
std::string  S100_LineSymbol::GetCrsType()     { return crsType; }
std::wstring S100_LineSymbol::GetCrsTypeW()    { return toWide(crsType); }
std::string  S100_LineSymbol::GetPosition()    { return position; }
std::wstring S100_LineSymbol::GetPositionW()   { return toWide(position); }
