#include "stdafx.h"
#include "S100_Include.h"

S100_Include::S100_Include()
{

}

S100_Include::~S100_Include()
{

}

void S100_Include::GetContents(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!pNode)
		return;

	MSXML2::IXMLDOMNamedNodeMapPtr pAttr = pNode->Getattributes();
	MSXML2::IXMLDOMNodePtr pAttrNP;
	pAttrNP = pAttr->getNamedItem(L"href");
	VARIANT value;
	if (pAttrNP)
	{
		pAttrNP->get_nodeValue(&value);
		href = std::wstring(value.bstrVal);
	}
}

void S100_Include::SetHerf(std::wstring& value)
{
	href = value;
}

std::wstring S100_Include::GetHerf()
{
	return href;
}