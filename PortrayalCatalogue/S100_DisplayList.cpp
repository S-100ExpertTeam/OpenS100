#include "stdafx.h"

#include "S100_DisplayList.h"
#include "S100_NullInstruction.h"
#include "S100_PointInstruction.h"
#include "S100_LineInstruction.h"
#include "S100_AreaInstruction.h"
#include "S100_TextInstruction.h"
#include "S100_AugmentedRay.h"
#include "S100_AugmentedPath.h"

S100_DisplayList::S100_DisplayList()
{
	displayInstructions.clear();
	alertInstructions.clear();
}

S100_DisplayList::~S100_DisplayList()
{
	RemoveInstructions();
}

void S100_DisplayList::OpenOutputXML(std::string path)
{
	pugi::xml_document doc;
	auto result = doc.load_file(path.c_str());
	auto displayListNode = doc.child("displayList");

	for each (auto child in displayListNode)
	{
		GetContents(child);
	}
}

void S100_DisplayList::GetContents(pugi::xml_node node)
{
	std::string nodeName = node.name();

	if (!nodeName.compare("nullInstruction"))
	{
		S100_NullInstruction* in = new S100_NullInstruction();
		in->GetContents(node);
		displayInstructions.push_back((S100_Instruction*)in);
	}
	else if (!nodeName.compare("pointInstruction"))
	{
		S100_PointInstruction* in = new S100_PointInstruction();
		in->GetContents(node);
		displayInstructions.push_back((S100_Instruction*)in);
	}
	else if (!nodeName.compare("lineInstruction"))
	{
		S100_LineInstruction* in = new S100_LineInstruction();
		in->GetContents(node);
		displayInstructions.push_back((S100_Instruction*)in);
	}
	else if (!nodeName.compare("areaInstruction"))
	{
		S100_AreaInstruction* in = new S100_AreaInstruction();
		in->GetContents(node);
		displayInstructions.push_back((S100_Instruction*)in);
	}
	else if (!nodeName.compare("textInstruction"))
	{
		S100_TextInstruction* in = new S100_TextInstruction();
		in->GetContents(node);
		displayInstructions.push_back((S100_Instruction*)in);
	}
	else if (!nodeName.compare("augmentedPoint"))
	{
	}
	else if (!nodeName.compare("augmentedRay"))
	{
		S100_AugmentedRay* in = new S100_AugmentedRay();
		in->GetContents(node);
		displayInstructions.push_back((S100_Instruction*)in);
	}
	else if (!nodeName.compare("augmentedPath"))
	{
		S100_AugmentedPath* in = new S100_AugmentedPath();
		in->GetContents(node);
		displayInstructions.push_back((S100_Instruction*)in);
	}
}

void S100_DisplayList::RemoveInstructions()
{
	for (auto iter : alertInstructions)
	{
		if (iter)
			delete iter, iter = nullptr;
	}
	alertInstructions.clear();

	for (auto iter : displayInstructions)
	{
		if (iter)
			delete iter, iter = nullptr;
	}
	displayInstructions.clear();
}

void S100_DisplayList::SetDisplayInstruction(S100_Instruction* value)
{
	displayInstructions.push_back(value);
}
void S100_DisplayList::SetDisplayInstructions(std::list<S100_Instruction*> value) {
	displayInstructions = value;
}

S100_Instruction* S100_DisplayList::GetDisplayInstruction(int index) {
	auto it = displayInstructions.begin();
	advance(it, index);
	return *it;
}
std::list<S100_Instruction*> S100_DisplayList::GetDisplayInstructions()
{
	return displayInstructions;
}
std::list<S100_Instruction*> S100_DisplayList::GetAlertInstructions()
{
	return alertInstructions;
}

void S100_DisplayList::AddAlertInstruction(S100_Instruction* _ins)
{
	alertInstructions.push_back(_ins);
}
