#include "stdafx.h"
#include "S100_HatchFill.h"

S100_HatchFill::S100_HatchFill()
{
	SetType(5);
}

S100_HatchFill::~S100_HatchFill()
{
	for (auto itor = hatches.begin(); itor != hatches.end(); itor++)
	{
		delete *itor;
	}
}

void S100_HatchFill::GetContents(MSXML2::IXMLDOMNodePtr pNode)
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

		if (nodeName.compare(L"hatch") == 0)
		{
			S100_Hatch* hatch = new S100_Hatch();
			hatch->GetContents(pNode);
			hatches.push_back(hatch);
		}
	}
}

void S100_HatchFill::GetContents(pugi::xml_node node)
{
	if (node==nullptr)
	{
		return;
	}

	((S100_PatternFill*)this)->GetContents(node);

	for (auto instruction=node.first_child(); instruction; instruction=instruction.next_sibling())
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName,"hatch")) 
		{
			S100_Hatch* hatch = new S100_Hatch();
			hatch->GetContents(instruction);
			hatches.push_back(hatch);
		}
	}
}

void S100_HatchFill::SetHatches(S100_Hatch* value) 
{
	hatches.push_back(value);
}

void S100_HatchFill::SetHatches(std::list<S100_Hatch*> value) 
{
	hatches = value;
}

S100_Hatch* S100_HatchFill::GetHatches(int index) 
{
	auto it = hatches.begin();
	advance(it, index);
	return *it;

}

std::list<S100_Hatch*> S100_HatchFill::GetHatches()
{
	return hatches;
}