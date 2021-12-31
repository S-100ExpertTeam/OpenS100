#include "stdafx.h"
#include "S100_PixmapFill.h"

S100_PixmapFill::S100_PixmapFill()
{
	SetType(3);
}

S100_PixmapFill::~S100_PixmapFill()
{

}

void S100_PixmapFill::GetContents(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!pNode)
		return;

	((S100_PatternFill*)this)->GetContents(pNode);

	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->GetchildNodes();

	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);

		if (!pNode)
			continue;

		std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();

		if (nodeName.compare(L"pixmap") == 0)
		{
			pixmap.GetContents(pNode->childNodes);
		}
	}
}

void S100_PixmapFill::GetContents(pugi::xml_node node)
{
	if (!node)
		return;

	((S100_PatternFill*)this)->GetContents(node);

	for (auto instruction=node.first_child(); instruction; instruction=instruction.next_sibling())
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName,"pixmap"))
		{
			pixmap.GetContents(instruction);
		}
	}
}

void S100_PixmapFill::SetPixmap(S100_Pixmap* value)
{
	pixmap = *value;
}

S100_Pixmap* S100_PixmapFill::GetPixmap() 
{
	return &pixmap;
}