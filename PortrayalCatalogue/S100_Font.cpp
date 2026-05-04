#include "stdafx.h"
#include "S100_Font.h"

S100_Font::S100_Font()
{

}

S100_Font::~S100_Font()
{

}

void S100_Font::GetContents(pugi::xml_node node)
{
	if (node == nullptr)
	{
		return;
	}

	for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName, "serifs"))
		{
			serifs = instruction.child_value();
		}
		else if (!strcmp(instructionName, "weight"))
		{
			weight = instruction.child_value();
		}
		else if (!strcmp(instructionName, "slant"))
		{
			slant = instruction.child_value();
		}
		else if (!strcmp(instructionName, "proportion"))
		{
			proportion = instruction.child_value();
		}
	}
}

void S100_Font::SetSerifs(const std::string& value)  { serifs = value; }
void S100_Font::SetSerifs(const std::wstring& value) { serifs = toUtf8(value); }
std::string  S100_Font::GetSerifs()  { return serifs; }
std::wstring S100_Font::GetSerifsW() { return toWide(serifs); }

void S100_Font::SetWeight(const std::string& value)  { weight = value; }
void S100_Font::SetWeight(const std::wstring& value) { weight = toUtf8(value); }
std::string  S100_Font::GetWeight()  { return weight; }
std::wstring S100_Font::GetWeightW() { return toWide(weight); }

void S100_Font::SetSlant(const std::string& value)  { slant = value; }
void S100_Font::SetSlant(const std::wstring& value) { slant = toUtf8(value); }
std::string  S100_Font::GetSlant()  { return slant; }
std::wstring S100_Font::GetSlantW() { return toWide(slant); }

void S100_Font::SetProportion(const std::string& value)  { proportion = value; }
void S100_Font::SetProportion(const std::wstring& value) { proportion = toUtf8(value); }
std::string  S100_Font::GetProportion()  { return proportion; }
std::wstring S100_Font::GetProportionW() { return toWide(proportion); }

bool S100_Font::isUpright()
{
	return slant.compare("Upright") == 0;
}
