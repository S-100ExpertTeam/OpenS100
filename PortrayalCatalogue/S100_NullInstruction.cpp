#include "stdafx.h"
#include "S100_NullInstruction.h"
#include "PortrayalCatalogue.h"


S100_NullInstruction::S100_NullInstruction()
{
	SetType(0);
}


S100_NullInstruction::~S100_NullInstruction()
{
}

void S100_NullInstruction::GetContents(MSXML2::IXMLDOMNodePtr pNode)
{
	if (!pNode)
		return;


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
		else
		{
			nodeName.append(L" : 없는 Context\n");
			OutputDebugString(nodeName.c_str());
		}
	}
}

void S100_NullInstruction::GetContents(pugi::xml_node node) 
{
	if (node==nullptr)
	{
		return;
	}

	for (auto instruction=node.first_child(); instruction; instruction=instruction.next_sibling())
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName,"featureReference")) 
		{
			GetFeatureReference()->GetContents(instruction);
		}
		else if (!strcmp(instructionName,"viewingGroup"))
		{
			SetViewingGroup(pugi::as_wide( instruction.child_value()));
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
		else 
		{
			std::wstring name =pugi::as_wide(instructionName);
			name.append(L"는 없는 Context");
		}
	}
}