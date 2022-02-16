#include "stdafx.h"
#include "S100_SymbolFill.h"

#include "..\\LibMFCUtil\\LibMFCUtil.h"

S100_SymbolFill::S100_SymbolFill()
{
	clipSymbols = L"true";
	symbol = NULL;

	v1 = NULL;
	v2 = NULL;
	
	SetType(4);
}

S100_SymbolFill::~S100_SymbolFill()
{
	if (symbol) delete symbol;

	if (v1) delete v1;
	if (v2) delete v2;
}

bool S100_SymbolFill::ReadFileByPugiXml(std::wstring& path)
{
	
	std::wstring head = L"..\\ProgramData\\S101_Portrayal\\AreaFills\\";

	path.insert(path.begin(), head.begin(), head.end());
	
	if (!path.empty())
	{
		 LibMFCUtil::GetFileName(path.c_str());

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(path.c_str());
		pugi::xml_node displayList = doc.child("symbolFill");

		if (displayList)
		{
			GetContents(displayList);
		}
		return true;
	}
	else
	{
		return false;
	}
}

void S100_SymbolFill::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}
	
	auto Attribute = node.attribute("clipSymbols"); //save attribute
	if (Attribute) 
	{
		clipSymbols =pugi::as_wide(Attribute.value());
	}

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "symbol"))
		{
			if (!symbol)
			{
				symbol = new S100_Symbol();
				symbol->GetContents(instruction);
			}
		}
		else if (!strcmp(instructionName, "v1"))
		{
			if (!v1)
			{
				v1 = new S100_VectorPoint();
				v1->GetContents(instruction);
			}
		}
		else if (!strcmp(instructionName, "v2"))
		{
			if (!v2)
			{
				v2 = new S100_VectorPoint();
				v2->GetContents(instruction);
			}
		}

	}
}

void S100_SymbolFill::SetClipSymbols(std::wstring& value)
{
	clipSymbols = value;
}
void S100_SymbolFill::SetSymbol(S100_Symbol* value) 
{
	symbol = value;
}
void S100_SymbolFill::SetV1(S100_VectorPoint* value) 
{
	v1 = value;
}
void S100_SymbolFill::SetV2(S100_VectorPoint* value)
{
	v2 = value;
}

std::wstring S100_SymbolFill::GetClipSymbols()
{
	return clipSymbols;
}
S100_Symbol* S100_SymbolFill::GetSymbol()
{
	return symbol;
}
S100_VectorPoint* S100_SymbolFill::GetV1()
{
	return v1;
}
S100_VectorPoint* S100_SymbolFill::GetV2()
{
	return v2;
}