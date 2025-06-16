#include "stdafx.h"
#include "S100_PointInstruction.h"

S100_PointInstruction::S100_PointInstruction()
{
	SetType(1);
	symbol = nullptr;
	vectorPoint = nullptr;
}

S100_PointInstruction::~S100_PointInstruction()
{
	if (symbol)
		delete symbol, symbol = nullptr;
	if (vectorPoint)
		delete vectorPoint, vectorPoint = nullptr;
}

void S100_PointInstruction::GetContents(pugi::xml_node node)
{
	if (node == nullptr)
	{
		return;
	}

	for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		auto instructionName = instruction.name();
		if (!strcmp(instructionName, "featureReference"))
		{
			SetFeatureReference(pugi::as_wide(instruction.child_value()));
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
			SetDrawingPriority(pugi::as_wide(instruction.child_value()));
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
				symbol = new S100_Symbol();
			symbol->GetContents(instruction);
		}
	}
}

void S100_PointInstruction::SetSymbol(S100_Symbol* value)
{
	symbol = value;
}

void S100_PointInstruction::SetSymbol(std::wstring& reference, double rotation)
{
	if (reference.empty())
		return;

	if (!symbol)
		symbol = new S100_Symbol();

	symbol->SetReference(reference);
	symbol->SetRotation(rotation);
}

void S100_PointInstruction::SetVectorPoint(S100_VectorPoint* value)
{
	vectorPoint = value;
}

void S100_PointInstruction::SetVectorPoint(std::wstring& x, std::wstring& y)
{
	if (!vectorPoint)
		vectorPoint = new S100_VectorPoint();

	vectorPoint->SetX(x);
	vectorPoint->SetY(y);
}

S100_Symbol* S100_PointInstruction::GetSymbol()
{
	return symbol;
}

S100_VectorPoint* S100_PointInstruction::GetVectorPoint()
{
	return vectorPoint;
}

