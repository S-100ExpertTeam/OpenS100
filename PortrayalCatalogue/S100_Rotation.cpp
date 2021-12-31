#include "stdafx.h"
#include "S100_Rotation.h"

S100_Rotation::S100_Rotation()
{

}

S100_Rotation::~S100_Rotation()
{

}

void S100_Rotation::GetContents(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!pNode)
		return;

	MSXML2::IXMLDOMNamedNodeMapPtr pAttr = pNode->Getattributes();
	MSXML2::IXMLDOMNodePtr pAttrNP;
	pAttrNP = pAttr->getNamedItem(L"useValueOf");
	VARIANT v;
	if (pAttrNP) {
		pAttrNP->get_nodeValue(&v);
		useValueOf = std::wstring(v.bstrVal);
	}
	value = std::wstring(pNode->Gettext());

	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->childNodes;
	if (!pNodeList)
		return;
	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNodeItem = pNodeList->Getitem(i);

		if (!pNodeItem)
			continue;

		std::wstring nodeName = (LPCTSTR)pNodeItem->GetnodeName();
	}
}
void S100_Rotation::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}

	auto Attribute = node.attribute("useValueOf"); //save atttribute
	if (Attribute)
	{
		value = pugi::as_wide(Attribute.value());
	}


	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "color")) 
		{
		}
	}

}

void S100_Rotation::SetUseValueOf(std::wstring& value)
{
	useValueOf = value;
}

void S100_Rotation::SetValue(std::wstring& v)
{
	value = v;
}

std::wstring S100_Rotation::GetUseValueOf() 
{
	return useValueOf;
}

std::wstring S100_Rotation::GetValue() 
{
	return value;
}