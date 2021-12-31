#include "stdafx.h"
#include "S100_Symbol.h"

S100_Symbol::S100_Symbol()
{
	rotation = NULL;
	areaPlacement = NULL;
	linePlacement = NULL;
}

S100_Symbol::~S100_Symbol()
{
	if (rotation) delete rotation;
	if (areaPlacement) delete areaPlacement;
	if (linePlacement) delete linePlacement;
}

void S100_Symbol::GetContents(MSXML2::IXMLDOMNodePtr pNode)
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
		reference = std::wstring(value.bstrVal);
	}
	pAttrNP = pAttr->getNamedItem(L"rotation");
	if (pAttrNP)
	{
		if (!rotation) rotation = new S100_Rotation();
		pAttrNP->get_nodeValue(&value);
		rotation->SetValue(std::wstring(value.bstrVal));
	}
	pAttrNP = pAttr->getNamedItem(L"scaleFactor");
	if (pAttrNP)
	{
		pAttrNP->get_nodeValue(&value);
		scaleFactor = std::wstring(value.bstrVal);
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->childNodes;
	if (!pNodeList)
		return;

	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);

		if (!pNode)
			continue;

		std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();

		if (nodeName.compare(L"symbolReference") == 0)
		{
			reference = std::wstring(pNode->Gettext());
		}
		else if (nodeName.compare(L"rotation") == 0)
		{
			if (!rotation) rotation = new S100_Rotation();
			rotation->GetContents(pNode);
		}
		else if (nodeName.compare(L"rotationCRS") == 0)
		{
			rotationCRS = std::wstring(pNode->Gettext());
		}
		else if (nodeName.compare(L"scaleFactor") == 0)
		{
			scaleFactor = std::wstring(pNode->Gettext());
		}
		else if (nodeName.compare(L"areaPlacement") == 0)
		{
			if (!areaPlacement) areaPlacement = new S100_AreaPlacement();
			areaPlacement->GetContents(pNode->childNodes);
		}
		else if (nodeName.compare(L"linePlacement") == 0)
		{
			if (!linePlacement) linePlacement = new S100_LinePlacement();
			linePlacement->GetContents(pNode->childNodes);
		}
	}
	return;
}
void S100_Symbol::GetContents(pugi::xml_node& node)
{
	if (!node)
	{
		return;
	}

	auto Attribute = node.attribute("reference"); // save attribute 
	if (Attribute)
	{
		reference = pugi::as_wide(Attribute.value());
	}
	
	Attribute = node.attribute("rotation"); // save attribute 
	if (Attribute)
	{
		reference = pugi::as_wide(Attribute.value());
	}

	Attribute = node.attribute("scaleFactor"); // save attribute 
	if (Attribute)
	{
		reference = pugi::as_wide(Attribute.value());
	}

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (instructionName == nullptr)
		{
			continue;
		}


		if (!strcmp(instructionName, "symbolReference"))
		{
			reference = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "rotation"))
		{
			if (!rotation)
			{
				rotation = new S100_Rotation();
				rotation->GetContents(instruction);
			}

		}
		else if (!strcmp(instructionName, "rotationCRS") == 0)
		{
			rotationCRS = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "scaleFactor") == 0)
		{
			scaleFactor = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "areaPlacement") == 0)
		{
			if (!areaPlacement)
			{
				areaPlacement = new S100_AreaPlacement();
				areaPlacement->GetContents(instruction);
			}

		}
		else if (!strcmp(instructionName, "linePlacement") == 0)
		{
			if (!linePlacement)
			{
				linePlacement = new S100_LinePlacement();
				linePlacement->GetContents(instruction);
			}
		}
		else
		{

		}
	}
}

void S100_Symbol::SetReference(std::wstring& value)
{
	reference = value;
}

void S100_Symbol::SetRotation(S100_Rotation* value) 
{
	rotation = value;
}

void S100_Symbol::SetRotationCRS(std::wstring& value)
{
	rotationCRS = value;
}

void S100_Symbol::SetScaleFactor(std::wstring& value)
{
	scaleFactor = value;
}

void S100_Symbol::SetAreaPlacement(S100_AreaPlacement* value) 
{
	areaPlacement = value;
}

void S100_Symbol::SetLinePlacement(S100_LinePlacement* value) 
{
	linePlacement = value;
}

std::wstring S100_Symbol::GetReference() 
{
	return reference;
}

S100_Rotation* S100_Symbol::GetRotation()
{
	return rotation;
}

std::wstring S100_Symbol::GetRotationCRS() 
{
	return rotationCRS;
}

std::wstring S100_Symbol::GetScaleFactor() 
{
	return scaleFactor;
}

S100_AreaPlacement* S100_Symbol::GetAreaPlacement()
{
	return areaPlacement;
}

S100_LinePlacement* S100_Symbol::GetLinePlacement()
{
	return linePlacement;
}
