#include "stdafx.h"
#include "S100_AreaFillReference.h"

S100_AreaFillReference::S100_AreaFillReference()
{
	SetType(2);
}

S100_AreaFillReference::~S100_AreaFillReference()
{

}

void S100_AreaFillReference::GetContents(MSXML2::IXMLDOMNodePtr pNode)
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

	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->GetchildNodes();

	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);

		if (!pNode)
			continue;
		std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();

		if (nodeName.compare(L"reference") == 0)
		{
			reference = pNode->Gettext();
		}
	}
}

void S100_AreaFillReference::GetContents(pugi::xml_node node) 
{
	if (node == nullptr) 
	{
		return;
	}

	for (auto attri=node.first_attribute(); attri; attri=attri.next_attribute())
	{
		auto attriName = attri.name();
		if (!strcmp(attriName,"reference"))
		{
			reference = pugi::as_wide(attri.value());
			break;
		}
	}

	for (auto instruction=node.first_child(); instruction; instruction=instruction.next_sibling())
	{
		auto instructionName = node.name();
		if (!strcmp(instructionName,"reference"))
		{
			reference = pugi::as_wide(instruction.value());
			break;
		}
	}
}

void S100_AreaFillReference::SetReference(std::wstring& value)
{
	reference = value;
}

std::wstring S100_AreaFillReference::GetReference() 
{
	return reference;
}