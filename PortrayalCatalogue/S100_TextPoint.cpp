#include "stdafx.h"
#include "S100_TextPoint.h"

S100_TextPoint::S100_TextPoint()
{
	offset = NULL;
	areaPlacement = NULL;

	horizontalAlignment = L"left";
	verticalAlignment = L"center";
}

S100_TextPoint::~S100_TextPoint()
{
	if (offset) delete offset;
	if (areaPlacement) delete areaPlacement;

	for (auto itor = elements.begin(); itor != elements.end(); itor++)
	{
		delete *itor;
	}
}

void S100_TextPoint::GetContents(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!pNode)
		return;
	MSXML2::IXMLDOMNamedNodeMapPtr pAttr = pNode->Getattributes();
	MSXML2::IXMLDOMNodePtr pAttrNP;
	VARIANT value;
	pAttrNP = pAttr->getNamedItem(L"horizontalAlignment");
	if (pAttrNP)
	{
		pAttrNP->get_nodeValue(&value);
		horizontalAlignment = std::wstring(value.bstrVal);
	}
	pAttrNP = pAttr->getNamedItem(L"verticalAlignment");
	if (pAttrNP)
	{
		pAttrNP->get_nodeValue(&value);
		verticalAlignment = std::wstring(value.bstrVal);
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->childNodes;
	if (!pNodeList)
		return;

	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pChildNode = pNodeList->Getitem(i);
		if (!pChildNode)
			continue;
		std::wstring nodeName = (LPCTSTR)pChildNode->GetnodeName();

		if (nodeName.compare(L"element") == 0)
		{
			S100_Element *element = new S100_Element();
			element->GetContents(pChildNode);
			elements.push_back(element);
		}
		else if (nodeName.compare(L"offset") == 0)
		{
			if (!offset) offset = new S100_VectorPoint();
			offset->GetContents(pChildNode);
		}
		else if (nodeName.compare(L"rotation") == 0)
		{
			rotation = std::wstring(pChildNode->Gettext());
		}
		else if (nodeName.compare(L"areaPlacement") == 0)
		{
			if (!areaPlacement) areaPlacement = new S100_AreaPlacement();
			areaPlacement->GetContents(pChildNode);
		}
	}
}
void S100_TextPoint::GetContents(pugi::xml_node node)
{
	if (node==nullptr) 
	{
		return;
	}
	for (auto attri=node.first_attribute(); attri; attri=attri.next_attribute()) 
	{
		auto attriName = attri.name();
		if (!strcmp(attriName,"horizontalAlignment"))
		{
			horizontalAlignment =pugi::as_wide( attri.value());
		}
		else if (!strcmp(attriName, "verticalAlignment"))
		{
			verticalAlignment = pugi::as_wide(attri.value());
		}
	}

	for (auto instruction=node.first_child(); instruction; instruction=instruction.next_sibling()) 
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName,"element"))
		{
			S100_Element *element = new S100_Element();
			element->GetContents(instruction);
			elements.push_back(element);
		}

		else if (!strcmp(instructionName, "offset"))
		{
			if (!offset) offset = new S100_VectorPoint();
			offset-> GetContents(instruction);
		}
		else if (!strcmp(instructionName,"rotation")) 
		{
			rotation = pugi::as_wide( instruction.child_value());
		}
		else if (!strcmp(instructionName, "areaPlacement"))
		{
			if (!areaPlacement) areaPlacement = new S100_AreaPlacement();
			areaPlacement->GetContents(instruction);
		}
	}
}

void S100_TextPoint::SetHorizontalAlignment(std::wstring& value)
{
	horizontalAlignment = value;
}

void S100_TextPoint::SetVerticalAlignment(std::wstring& value)
{
	verticalAlignment = value;
}

void S100_TextPoint::SetElement(S100_Element* value) 
{
	elements.push_back(value);
}

void S100_TextPoint::SetElements(std::vector<S100_Element*> value) 
{
	elements = value;
}

void S100_TextPoint::SetOffset(S100_VectorPoint* value) 
{
	offset = value;
}

void S100_TextPoint::SetRotation(std::wstring value) 
{
	rotation = value;
}


void S100_TextPoint::SetAreaPlacement(S100_AreaPlacement* value) 
{
	areaPlacement = value;
}

std::wstring S100_TextPoint::GetHorizontalAlignment() 
{
	return horizontalAlignment;
}

std::wstring S100_TextPoint::GetVerticalAlignment() 
{
	return verticalAlignment;
}

S100_Element* S100_TextPoint::GetElement(int index)
{
	return elements.at(index);
}

std::vector<S100_Element*> S100_TextPoint::GetElemets() 
{
	return elements;
}

S100_VectorPoint* S100_TextPoint::GetOffset() 
{
	return offset;
}

std::wstring S100_TextPoint::GetRotation() 
{
	return rotation;
}

S100_AreaPlacement* S100_TextPoint::GetAreaPlacement() 
{
	return areaPlacement;
}

