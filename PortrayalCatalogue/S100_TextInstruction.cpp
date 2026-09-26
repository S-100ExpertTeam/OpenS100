#include "stdafx.h"
#include "S100_TextInstruction.h"
#include "PortrayalCatalogue.h"

S100_TextInstruction::S100_TextInstruction()
{
	SetType(5);
	textPoint = NULL;
}


S100_TextInstruction::~S100_TextInstruction()
{
	if (textPoint)
		delete textPoint;
}

void S100_TextInstruction::GetContents(pugi::xml_node node)
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
		else if (!strcmp(instructionName, "viewingGroup"))
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
		else if (!strcmp(instructionName, "textPoint"))
		{
			if (!textPoint) textPoint = new S100_TextPoint();
			textPoint->GetContents(instruction);
		}
	}
}

void S100_TextInstruction::SetTextPoint(S100_TextPoint* value) { textPoint = value; }
S100_TextPoint* S100_TextInstruction::GetTextPoint()          { return textPoint; }
