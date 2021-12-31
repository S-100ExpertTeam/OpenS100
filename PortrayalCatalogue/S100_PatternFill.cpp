#include "stdafx.h"
#include "S100_PatternFill.h"

S100_PatternFill::S100_PatternFill()
{

}

S100_PatternFill::~S100_PatternFill()
{

}

void S100_PatternFill::GetContents(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!pNode)
		return;

	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->GetchildNodes();

	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);

		if (!pNode)
			continue;

		std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();

		if (nodeName.compare(L"areaCRS") == 0)
		{
			areaCRS = std::wstring(pNode->Gettext());
		}
	}
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
			areaCRS = pugi::as_wide(instruction.child_value());
			break;
		}
	}
}

void S100_PatternFill::SetAreaCRS(std::wstring& value)
{
	areaCRS = value;
}

std::wstring S100_PatternFill::GetAreaCRS()
{
	return areaCRS;
}