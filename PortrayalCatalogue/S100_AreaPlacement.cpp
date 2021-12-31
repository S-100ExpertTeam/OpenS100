#include "stdafx.h"
#include "S100_AreaPlacement.h"


S100_AreaPlacement::S100_AreaPlacement()
{
}


S100_AreaPlacement::~S100_AreaPlacement()
{

}

void S100_AreaPlacement::GetContents(MSXML2::IXMLDOMNodeListPtr pNodeList)
{
	if (!pNodeList)
		return;

	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);

		if (!pNode)
			continue;

		std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();

		if (nodeName.compare(L"placementMode") == 0)
		{
			placementMode = std::wstring(pNode->Gettext());
		}
	}
}
void S100_AreaPlacement::GetContents(pugi::xml_node& node) 
{
	if (!node)
		return;

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		const pugi::char_t* instructionName = instruction.name();
		if (!strcmp(instructionName, "placementMode"))
		{
			placementMode = pugi::as_wide(instruction.child_value());
		}
	}
}

void S100_AreaPlacement::SetPlacementMode(std::wstring& value)
{
	placementMode = value;
}

std::wstring S100_AreaPlacement::GetPlacementMode() 
{
	return placementMode;
}