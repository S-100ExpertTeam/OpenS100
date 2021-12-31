#include "stdafx.h"
#include "S100_Text.h"

S100_Text::S100_Text()
{

}

S100_Text::~S100_Text()
{

}

void S100_Text::GetContents(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!pNode)
		return;
	MSXML2::IXMLDOMNamedNodeMapPtr pAttr = pNode->Getattributes();
	MSXML2::IXMLDOMNodePtr pAttrNP;
	pAttrNP = pAttr->getNamedItem(L"useValueOf");
	VARIANT variant;
	if (pAttrNP)
	{
		pAttrNP->get_nodeValue(&variant);
		useValueOf = std::wstring(variant.bstrVal);
	}

	value = pNode->Gettext().GetBSTR();

	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->childNodes;
	if (!pNodeList)
		return;
	for (int i = 0; i < pNodeList->Getlength(); i++) 
	{
		MSXML2::IXMLDOMNodePtr pChildNode = pNodeList->Getitem(i);
		if (!pChildNode)
			continue;
		std::wstring nodeName = (LPCTSTR)pChildNode->GetnodeName();
	}
}

void S100_Text::GetContents(pugi::xml_node node)
{
	if (node == nullptr)
	{
		return;
	}

	for (auto attri = node.first_attribute(); attri; attri = attri.next_attribute())
	{
		auto attriName = attri.name();
		if (!strcmp(attriName, "useValueOf"))
		{
			useValueOf = pugi::as_wide(attri.value());
			break;
		}
	}
}

void S100_Text::SetValue(std::wstring& _value)
{
	value = _value;
}

void S100_Text::SetUseValueOf(std::wstring& value)
{
	useValueOf = value;
}

std::wstring S100_Text::GetValue() 
{
	return value;
}

std::wstring S100_Text::GetUseValueOf() 
{
	return useValueOf;
}