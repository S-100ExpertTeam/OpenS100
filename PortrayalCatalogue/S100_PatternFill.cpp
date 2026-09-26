#include "stdafx.h"
#include "S100_PatternFill.h"

S100_PatternFill::S100_PatternFill()
{

}

S100_PatternFill::~S100_PatternFill()
{

}

void S100_PatternFill::GetContents(pugi::xml_node node)
{
	if (node==nullptr)
	{
		return;
	}

	for (auto instruction = node.first_child(); instruction; instruction=instruction.next_sibling())
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName,"areaCRS"))
		{
			areaCRS = instruction.child_value();
			break;
		}
	}
}

void S100_PatternFill::SetAreaCRS(const std::string& value)  { areaCRS = value; }
void S100_PatternFill::SetAreaCRS(const std::wstring& value) { areaCRS = toUtf8(value); }
std::string  S100_PatternFill::GetAreaCRS()  { return areaCRS; }
std::wstring S100_PatternFill::GetAreaCRSW() { return toWide(areaCRS); }
