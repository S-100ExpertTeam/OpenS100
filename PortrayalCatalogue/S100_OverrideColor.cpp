#include "stdafx.h"
#include "S100_OverrideColor.h"

S100_OverrideColor::S100_OverrideColor()
{

}

S100_OverrideColor::~S100_OverrideColor()
{

}

void S100_OverrideColor::GetContents(MSXML2::IXMLDOMNodePtr pNode)
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

		if (nodeName.compare(L"override") == 0)
		{
			overrideColor.GetContents(pNode);
		}
		else if (nodeName.compare(L"color") == 0)
		{
			color.GetContents(pNode);
		}
	}
}

void S100_OverrideColor::GetContents(pugi::xml_node node) 
{
	if (node==nullptr)
	{
		return;
	}


	for (auto instruction=node.first_child(); instruction; instruction=instruction.next_sibling()) 
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName,"override"))
		{
			overrideColor.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "color"))
		{
			color.GetContents(instruction);
		}
	}
}

void S100_OverrideColor::SetOverrideColor(S100_Color* value) 
{
	overrideColor = *value;
}

S100_Color* S100_OverrideColor::GetOverrideColor() 
{
	return &overrideColor;
}

void S100_OverrideColor::SetColor(S100_Color* value) 
{
	color = *value;
}

S100_Color* S100_OverrideColor::GetColor() 
{
	return &color;
}