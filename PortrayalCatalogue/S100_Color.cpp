#include "stdafx.h"
#include "S100_Color.h"

S100_Color::S100_Color()
{

}

S100_Color::~S100_Color()
{

}

void S100_Color::GetContents(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!pNode)
	{
		return;
	}

	MSXML2::IXMLDOMNamedNodeMapPtr pAttr = pNode->Getattributes();
	if (pAttr)
	{
		MSXML2::IXMLDOMNodePtr pAttrNP;
		VARIANT value;

		pAttrNP = pAttr->getNamedItem(L"token");
		if (pAttrNP) {
			pAttrNP->get_nodeValue(&value);
			token = std::wstring(value.bstrVal);
		}
		pAttrNP = pAttr->getNamedItem(L"name");
		if (pAttrNP) {
			pAttrNP->get_nodeValue(&value);
			name = std::wstring(value.bstrVal);
		}
		pAttrNP = pAttr->getNamedItem(L"transparency");
		if (pAttrNP) {
			pAttrNP->get_nodeValue(&value);
			transparency = std::wstring(value.bstrVal);
		}
	}
	else
	{
		token = pNode->Gettext(); 
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->childNodes;
	if (!pNodeList)
		return;

	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode3 = pNodeList->Getitem(i);

		if (!pNode3)
			continue;

		std::wstring nodeName = (LPCTSTR)pNode3->GetnodeName();

		if (nodeName.compare(L"#text") == 0)
		{
			token = std::wstring(pNode3->Gettext());
		}
		else if (nodeName.compare(L"token") == 0)
		{
			token = std::wstring(pNode3->Gettext());
		}
		else if (nodeName.compare(L"transparency") == 0)
		{
		}
		else if (nodeName.compare(L"description") == 0)
		{
			description = std::wstring(pNode3->Gettext());
		}
	}
}
void S100_Color::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}

	for (auto colorNode = node.first_attribute(); colorNode; colorNode = colorNode.next_attribute())
	{
		auto colorNodeName = colorNode.name();
		if (!strcmp(colorNodeName, "token"))
		{
			token = pugi::as_wide(colorNode.value());
		}
		else if (!strcmp(colorNodeName, "name"))
		{
			name = pugi::as_wide(colorNode.value());
		}
	}

	for (auto instruction= node.first_child(); instruction; instruction= instruction.next_sibling()) 
	{
		auto instructionName = instruction.name(); 
		if (!strcmp(instructionName,"description"))
		{
			description = pugi::as_wide(instruction.child_value());
		}
	}
}

void S100_Color::SetToken(std::wstring& value)
{
	token = value;
}

std::wstring S100_Color::GetToken() 
{
	return token;
}

void S100_Color::SetName(std::wstring& value)
{
	name = value;
}

std::wstring S100_Color::GetName() 
{
	return name;
}

void S100_Color::SetDescription(std::wstring& value)
{
	description = value;
}

std::wstring S100_Color::GetDescription()
{
	return description;
}

void S100_Color::SetTransparency(std::wstring& value)
{
	transparency = value;
}

std::wstring S100_Color::GetTransparency()
{
	return transparency;
}