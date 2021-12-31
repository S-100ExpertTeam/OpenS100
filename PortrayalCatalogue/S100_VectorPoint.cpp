#include "stdafx.h"
#include "S100_VectorPoint.h"

S100_VectorPoint::S100_VectorPoint()
{

}

S100_VectorPoint::~S100_VectorPoint()
{

}

void S100_VectorPoint::GetContents(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!pNode)
		return;

	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->childNodes;
	if (!pNodeList)
		return;
	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pChildNode = pNodeList->Getitem(i);

		if (!pChildNode)
			continue;

		std::wstring nodeName = (LPCTSTR)pChildNode->GetnodeName();

		if (nodeName.compare(L"x") == 0)
		{
			x = std::wstring(pChildNode->Gettext());
		}
		else if (nodeName.compare(L"y") == 0)
		{
			y = std::wstring(pChildNode->Gettext());
		}
	}
}
void S100_VectorPoint::GetContents(pugi::xml_node& node)
{
	if (!node)
		return;

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		if (!instruction) {
			continue;
		}

		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "x"))
		{
			x = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "y"))
		{
			y = pugi::as_wide(instruction.child_value());
		}
	}
}

void S100_VectorPoint::SetX(std::wstring& value)
{
	x = value;
}

void S100_VectorPoint::SetY(std::wstring& value)
{
	y = value;
}

std::wstring S100_VectorPoint::GetX() 
{
	return x;
}

std::wstring S100_VectorPoint::GetY() 
{
	return y;
}