#include "stdafx.h"
#include "S100_Pixmap.h"

S100_Pixmap::S100_Pixmap()
{

}

S100_Pixmap::~S100_Pixmap()
{

}

void S100_Pixmap::GetContents(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!pNode)
		return;

	MSXML2::IXMLDOMNamedNodeMapPtr pAttr = pNode->Getattributes();
	MSXML2::IXMLDOMNodePtr pAttrNP;
	VARIANT value;

	pAttrNP = pAttr->getNamedItem(L"reference");
	if (pAttrNP)
	{
		pAttrNP->get_nodeValue(&value);
		reference = std::wstring(value.bstrVal).c_str();
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->GetchildNodes();

	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);

		if (!pNode)
			continue;

		std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();

		if (nodeName.compare(L"overrideAll") == 0)
		{
			overrideAll.GetContents(pNode);
		}
		else if (nodeName.compare(L"override") == 0)
		{
			overrideColor.GetContents(pNode);
		}
	}
}

void S100_Pixmap::GetContents(pugi::xml_node node) 
{
	if (node==nullptr)
	{
		return;
	}
	
	for (auto instruction= node.first_child(); instruction; instruction=instruction.next_sibling()) 
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName,"overrideAll"))
		{
			overrideAll.GetContents(instruction);
		}
		else if (!strcmp(instructionName, "override"))
		{
			overrideColor.GetContents(instruction);
		}
	}
}

void S100_Pixmap::SetReference(std::wstring& value)
{
	reference = value;
}

std::wstring S100_Pixmap::GetReference() 
{
	return reference;
}

void S100_Pixmap::SetOverrideAll(S100_Color* value) 
{
	overrideAll = *value;
}

S100_Color* S100_Pixmap::GetOverrideAll() 
{
	return &overrideAll;
}

void S100_Pixmap::SetOverrideColor(S100_OverrideColor* value) 
{
	overrideColor = *value;
}

S100_OverrideColor* S100_Pixmap::GetOverrideColor() 
{
	return &overrideColor;
}