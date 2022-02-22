#include "stdafx.h"
#include "S100_PointInstruction.h"

S100_PointInstruction::S100_PointInstruction() 
{
	SetType(1);
	symbol = NULL;
	vectorPoint = NULL;
}

S100_PointInstruction::~S100_PointInstruction()
{
	if (symbol) delete symbol;
	if (vectorPoint) delete vectorPoint;
}

void S100_PointInstruction::GetContents(pugi::xml_node node)
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
		else if (!strcmp(instructionName, "symbol"))
		{
			if (!symbol)
			{
				symbol = new S100_Symbol();
				symbol->GetContents(instruction);
			}
		}

	}
}

void S100_PointInstruction::SetSymbol(S100_Symbol* value) 
{
	symbol = value;
}

void S100_PointInstruction::SetVectorPoint(S100_VectorPoint* value) 
{
	vectorPoint = value;
}

S100_Symbol* S100_PointInstruction::GetSymbol() 
{
	return symbol;
}

S100_VectorPoint* S100_PointInstruction::GetVectorPoint() 
{
	return vectorPoint;
}