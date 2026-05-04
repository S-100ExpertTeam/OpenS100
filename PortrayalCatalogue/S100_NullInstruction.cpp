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
			SetFeatureReference(std::string(instruction.child_value()));
		}
		else if (!strcmp(instructionName,"viewingGroup"))
		{
			SetViewingGroup(std::string(instruction.child_value()));
		}
		else if (!strcmp(instructionName, "displayPlane"))
		{
			SetDisplayPlane(std::string(instruction.child_value()));
		}
		else if (!strcmp(instructionName, "drawingPriority"))
		{
			SetDrawingPriority(std::string(instruction.child_value()));
		}
		else if (!strcmp(instructionName, "scaleMinimum"))
		{
			SetScaleMinimum(std::string(instruction.child_value()));
		}
		else if (!strcmp(instructionName, "scaleMaximum"))
		{
			SetScaleMaximum(std::string(instruction.child_value()));
		}
	}
}
