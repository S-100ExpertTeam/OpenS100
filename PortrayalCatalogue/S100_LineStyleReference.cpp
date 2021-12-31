#include "stdafx.h"
#include "S100_LineStyleReference.h"

S100_LineStyleReference::S100_LineStyleReference()
{
	SetType(2);
}

S100_LineStyleReference::~S100_LineStyleReference()
{

}

void S100_LineStyleReference::GetContents(MSXML2::IXMLDOMNodePtr pNode)
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

		if (nodeName.compare(L"reference") == 0)
		{
			reference = pNode->Gettext();
		}
	}
}
void S100_LineStyleReference::GetContents(pugi::xml_node& node)
{
	if (!node) 
	{
		return;
	}


	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "reference"))
		{
			reference = pugi::as_wide(instruction.child_value());
		}
	}
}

void S100_LineStyleReference::SetReference(std::wstring& value)
{
	reference = value;
}
std::wstring S100_LineStyleReference::GetReference() 
{
	return reference;
}