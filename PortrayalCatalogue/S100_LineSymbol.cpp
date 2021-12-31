#include "stdafx.h"
#include "S100_LineSymbol.h"

S100_LineSymbol::S100_LineSymbol()
{
	//reference;
	rotation = L"0.0";
	scaleFactor = L"1.0";
	crsType = L"LocalCRS";
}

S100_LineSymbol::~S100_LineSymbol()
{

}

void S100_LineSymbol::GetContents(MSXML2::IXMLDOMNodePtr pNode)
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
	pAttrNP = pAttr->getNamedItem(L"rotation");
	if (pAttrNP)
	{
		pAttrNP->get_nodeValue(&value);
		rotation = std::wstring(value.bstrVal).c_str();
	}
	pAttrNP = pAttr->getNamedItem(L"scaleFactor");
	if (pAttrNP)
	{
		pAttrNP->get_nodeValue(&value);
		scaleFactor = std::wstring(value.bstrVal).c_str();
	}
	pAttrNP = pAttr->getNamedItem(L"crsType");
	if (pAttrNP)
	{
		pAttrNP->get_nodeValue(&value);
		crsType = std::wstring(value.bstrVal).c_str();
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->GetchildNodes();

	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);

		if (!pNode)
			continue;

		std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();

		if (nodeName.compare(L"position") == 0)
		{
			position = std::wstring(pNode->Gettext());
		}
	}
}
void S100_LineSymbol::GetContents(pugi::xml_node& node) 
{
	if (!node)
	{
		return;
	}
	for each (pugi::xml_attribute attri in node.attributes())
	{
		auto attriName = attri.name();
		if (!strcmp(attriName, "reference"))
		{
			reference = pugi::as_wide(attri.value());
		}
		else if (!strcmp(attriName, "rotation"))
		{
			rotation = pugi::as_wide(attri.value());
		}
		else if (!strcmp(attriName, "scaleFactor"))
		{
			scaleFactor = pugi::as_wide(attri.value());
		}
		else if (!strcmp(attriName, "crsType"))
		{
			crsType = pugi::as_wide(attri.value());
		}
	}



	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();

		if (!strcmp(instructionName, "position"))
		{
			position = pugi::as_wide(instruction.child_value());
		}
	}
}

void S100_LineSymbol::SetReference(std::wstring& value)
{
	reference = value;
}

void S100_LineSymbol::SetRotation(std::wstring& value)
{
	rotation = value;
}

void S100_LineSymbol::SetScaleFactor(std::wstring& value)
{
	scaleFactor = value;
}

void S100_LineSymbol::SetCrsType(std::wstring& value)
{
	crsType = value;
}

void S100_LineSymbol::SetPosition(std::wstring& value)
{
	position = value;
}

std::wstring S100_LineSymbol::GetReference() 
{
	return reference;
}

std::wstring S100_LineSymbol::GetRotation() 
{
	return rotation;
}

std::wstring S100_LineSymbol::GetScaleFactor()
{
	return scaleFactor;
}

std::wstring S100_LineSymbol::GetCrsType() 
{
	return crsType;
}

std::wstring S100_LineSymbol::GetPosition() 
{
	return position;
}