#include "stdafx.h"
#include "S100_Font.h"

S100_Font::S100_Font()
{

}

S100_Font::~S100_Font()
{

}

void S100_Font::GetContents(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!pNode)
		return;

	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->childNodes;
	if (!pNodeList)
		return;
	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pChildNode = pNodeList->Getitem(i);
		if (!pChildNode)
			continue;
		std::wstring nodeName = (LPCTSTR)pChildNode->GetnodeName();

		if (nodeName.compare(L"serifs") == 0)
		{
			serifs = std::wstring(pChildNode->Gettext());
		}
		else if (nodeName.compare(L"weight") == 0)
		{
			weight = std::wstring(pChildNode->Gettext());
		}
		else if (nodeName.compare(L"slant") == 0)
		{
			slant = std::wstring(pChildNode->Gettext());
		}
		else if (nodeName.compare(L"proportion") == 0)
		{
			proportion = std::wstring(pChildNode->Gettext());
		}
	}
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