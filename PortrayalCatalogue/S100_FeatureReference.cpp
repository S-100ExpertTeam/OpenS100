#include "stdafx.h"
#include "S100_FeatureReference.h"

S100_FeatureReference::S100_FeatureReference()
{

}

S100_FeatureReference::~S100_FeatureReference()
{

}

void S100_FeatureReference::GetContents(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!pNode)
		return;
	MSXML2::IXMLDOMNamedNodeMapPtr pAttr = pNode->Getattributes();
	MSXML2::IXMLDOMNodePtr pAttrNP;
	pAttrNP = pAttr->getNamedItem(L"reference");
	VARIANT value;
	if (pAttrNP)
	{
		pAttrNP->get_nodeValue(&value);
		reference = std::wstring(value.bstrVal);
	}
	else
	{
		reference = pNode->Gettext();
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->childNodes;
	if (!pNodeList)
		return;
	for (int i = 0; i < pNodeList->Getlength(); i++) 
	{
		MSXML2::IXMLDOMNodePtr pNode2 = pNodeList->Getitem(i);
		if (!pNode2)
			continue;
		std::wstring nodeName = (LPCTSTR)pNode->GetnodeName(); 
	}
}

void S100_FeatureReference::GetContents(pugi::xml_node node)
{
	if (node == nullptr)
	{
		return;
	}

	for (auto attri = node.first_attribute(); attri; attri = attri.next_attribute())
	{
		auto attriName = attri.name();
		if (!strcmp(attriName, "reference"))
		{
			reference = pugi::as_wide(attri.value());
		}
		reference = pugi::as_wide(node.child_value());
	}

}

void S100_FeatureReference::SetReference(std::wstring& value)
{
	reference = value;
}

std::wstring S100_FeatureReference::GetReference()
{
	return reference;
}