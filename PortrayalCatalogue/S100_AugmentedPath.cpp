#include "stdafx.h"
#include "S100_AugmentedPath.h"
#include "PortrayalCatalogue.h"

S100_AugmentedPath::S100_AugmentedPath()
{
	SetType(8);
	path = NULL;
}

S100_AugmentedPath::~S100_AugmentedPath()
{
	if (path)
		delete path;
}

void S100_AugmentedPath::GetContents(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!pNode)
		return;

	MSXML2::IXMLDOMNamedNodeMapPtr pAttr = pNode->Getattributes();
	MSXML2::IXMLDOMNodePtr pAttrNP;
	VARIANT value;

	pAttrNP = pAttr->getNamedItem(L"crsType");
	if (pAttrNP)
	{
		pAttrNP->get_nodeValue(&value);

		std::wstring bstrVal = value.bstrVal;
		std::wstring& reuslt = bstrVal;
		SetCrsType(reuslt);
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList = pNode->GetchildNodes();


	for (int i = 0; i < pNodeList->Getlength(); i++)
	{
		MSXML2::IXMLDOMNodePtr pNode = pNodeList->Getitem(i);

		if (!pNode)
			continue;

		std::wstring nodeName = (LPCTSTR)pNode->GetnodeName();

		if (nodeName.compare(L"featureReference") == 0)
		{
			GetFeatureReference()->GetContents(pNode);
		}
		else if (nodeName.compare(L"viewingGroup") == 0)
		{
			SetViewingGroup(std::wstring(pNode->Gettext()));
		}
		else if (nodeName.compare(L"displayPlane") == 0)
		{
			SetDisplayPlane(std::wstring(pNode->Gettext()));
		}
		else if (nodeName.compare(L"drawingPriority") == 0)
		{
			SetDrawingProiority(std::wstring(pNode->Gettext()));
		}
		else if (nodeName.compare(L"scaleMinimum") == 0)
		{
			SetScaleMinimum(std::wstring(pNode->Gettext()));
		}
		else if (nodeName.compare(L"scaleMaximum") == 0)
		{
			SetScaleMaximum(std::wstring(pNode->Gettext()));
		}
		else if (nodeName.compare(L"path") == 0)
		{
			if (!path) path = new S100_Path();
			path->GetContents(pNode);
		}
		else if (nodeName.compare(L"lineStyle") == 0)
		{
			if (!GetLineStyle()) SetLineStyle(new S100_LineStyle());
			GetLineStyle()->GetContents(pNode);
		}
		else if (nodeName.compare(L"lineStyle") == 0)
		{
			if (!GetLineStyle()) SetLineStyle(new S100_LineStyle());
			GetLineStyle()->GetContents(pNode);
		}
		else if (nodeName.compare(L"lineStyleReference") == 0)
		{
			if (!GetLineStyleReference()) SetLineStyleReference(new S100_LineStyleReference());
			GetLineStyleReference()->GetContents(pNode);
		}
		else if (nodeName.compare(L"compositeLineStyle") == 0)
		{
			if (!GetCompositeLineStyle()) SetCompositeLineStyle(new S100_CompositeLineStyle());
			GetCompositeLineStyle()->GetContents(pNode);
		}
		else if (nodeName.compare(L"textPoint") == 0)
		{
			if (!GetTextPoint()) SetTextPoint(new S100_TextPoint());
			GetTextPoint()->GetContents(pNode);
		}
		else if (nodeName.compare(L"crs") == 0)
		{
			SetCrsType(std::wstring(pNode->Gettext()));
		}
	}
}

void S100_AugmentedPath::GetContents(pugi::xml_node node)
{

	for (auto instruction = node.first_child(); instruction; instruction = instruction)
	{
		auto instructionName = instruction.name();

		if (!strcmp(instructionName, "featureReference"))
		{
			GetFeatureReference()->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "viewingGroup"))
		{
			SetViewingGroup(pugi::as_wide(instruction.child_value()));
		}
		else if (!strcmp(instructionName, "displayPlane"))
		{
			SetDisplayPlane(pugi::as_wide(instruction.child_value()));
		}
		else if (!strcmp(instructionName, "drawingPriority"))
		{
			SetDrawingProiority(pugi::as_wide(instruction.child_value()));
		}
		else if (!strcmp(instructionName, "scaleMinimum"))
		{
			SetScaleMinimum(pugi::as_wide(instruction.child_value()));
		}
		else if (!strcmp(instructionName, "scaleMaximum"))
		{
			SetScaleMaximum(pugi::as_wide(instruction.child_value()));
		}
		else if (!strcmp(instructionName, "path"))
		{
			if (!path) path = new S100_Path();
			path->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "lineStyle"))
		{
			if (!GetLineStyle()) SetLineStyle(new S100_LineStyle());
			GetLineStyle()->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "lineStyleReference"))
		{
			if (!GetLineStyleReference()) SetLineStyleReference(new S100_LineStyleReference());
			GetLineStyleReference()->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "compositeLineStyle"))
		{
			if (!GetCompositeLineStyle())SetCompositeLineStyle(new S100_CompositeLineStyle());
			GetCompositeLineStyle()->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "textPoint"))
		{
			if (!GetTextPoint()) SetTextPoint(new S100_TextPoint());
			GetTextPoint()->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "crs"))
		{
			SetCrsType(pugi::as_wide(instruction.child_value()));
		}
	}
}

void S100_AugmentedPath::SetPath(S100_Path* value)
{
	path = value;
}

S100_Path* S100_AugmentedPath::GetPath()
{
	return path;
}