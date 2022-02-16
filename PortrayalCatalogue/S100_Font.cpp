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
	if (node==nullptr)
	{
		return;
	}

	for (auto instruction=node.first_child(); instruction; instruction=instruction.next_sibling()) 
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName,"serifs"))
		{
			serifs = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "weight"))
		{
			weight = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "slant"))
		{
			slant = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "proportion"))
		{
			proportion = pugi::as_wide(instruction.child_value());
		}
	}
}

void S100_Font::SetSerifs(std::wstring& value)
{
	serifs = value;
}

std::wstring S100_Font::GetSerifs() 
{
	return serifs;
}

void S100_Font::SetWeight(std::wstring& value)
{
	weight = value;
}

std::wstring S100_Font::GetWeight()
{
	return weight;
}

void S100_Font::SetSlant(std::wstring& value)
{
	slant = value;
}

std::wstring S100_Font::GetSlant()
{
	return slant;
}

void S100_Font::SetProportion(std::wstring& value)
{
	proportion = value;
}

std::wstring S100_Font::GetProportion()
{
	return proportion;
}