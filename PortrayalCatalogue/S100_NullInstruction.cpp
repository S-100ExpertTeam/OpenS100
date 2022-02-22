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
			name.append(L" Context without");
		}
	}
}