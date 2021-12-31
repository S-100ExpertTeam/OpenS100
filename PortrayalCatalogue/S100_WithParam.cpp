#include "stdafx.h"
#include "S100_WithParam.h"

S100_WithParam::S100_WithParam()
{

}

S100_WithParam::~S100_WithParam()
{

}

void S100_WithParam::GetContents(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!pNode)
		return;

	MSXML2::IXMLDOMNamedNodeMapPtr pAttr = pNode->Getattributes();
	MSXML2::IXMLDOMNodePtr pAttrNP;
	VARIANT v;

	pAttrNP = pAttr->getNamedItem(L"name");
	if (pAttrNP)
	{
		pAttrNP->get_nodeValue(&v);
		name = std::wstring(v.bstrVal);
	}
	pAttrNP = pAttr->getNamedItem(L"select");
	if (pAttrNP)
	{
		pAttrNP->get_nodeValue(&v);
		select = std::wstring(v.bstrVal);
	}

	value = std::wstring(pNode->Gettext());

	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->childNodes;
	if (!pNodeList)
		return;
	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);

		if (!pNode)
			continue;

		std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();

	}
}

void S100_WithParam::SetName(std::wstring& value)
{
	name = value;
}

void S100_WithParam::SetSelect(std::wstring& value)
{
	select = value;
}

void S100_WithParam::SetValue(std::wstring& _value)
{
	value = _value;
}

std::wstring S100_WithParam::GetName() 
{
	return name;
}

std::wstring S100_WithParam::GetSelect() 
{
	return select;
}

std::wstring S100_WithParam::GetValue()
{
	return value;
}