#include "stdafx.h"
#include "CommandToInstruction.h"

#include "..\\PortrayalCatalogue\\S100_AugmentedRay.h"

S100_PointInstruction* CommandToInstruction::ToS100PointInstruction(Local_DrawingCommands& dc, Local_StateCommands& sc)
{
	S100_Instruction* instruction = nullptr;
	if (sc.augmentedPoint.isPresent())
	{
		instruction = new S100_AugmentedPoint();
	}
	else
	{
		instruction = new S100_PointInstruction();
	}

	// DrawingInstruction
	SetDrawingInstruction(sc, instruction);

	// PointInstruction
	if (dc.pointInstruction.isPresent())
	{
		auto pointInstruction = (S100_PointInstruction*)instruction;

		// Symbol::Symbol
		S100_Symbol* symbol = new S100_Symbol();
		pointInstruction->SetSymbol(symbol);
		
		// reference
		symbol->SetReference(dc.pointInstruction.GetSymbol());

		// rotation
		// rotationCRS
		if (sc.rotation.isPresent())
		{
			symbol->SetRotation(sc.rotation.GetRotation());
			symbol->SetRotationCRS(sc.rotation.GetRotationCRS());
		}

		// scaleFactor
		if (sc.scaleFactor.isPresent())
		{
			symbol->SetScaleFactor(sc.scaleFactor.GetScaleFactor());
		}

		// offset
		if (sc.localOffset.isPresent())
		{
			symbol->SetOffsetX(sc.localOffset.GetXOffsetMM());
			symbol->SetOffsetY(sc.localOffset.GetYOffsetMM());
		}

		// linePlacement
		if (sc.linePlacement.isPresent())
		{
			symbol->SetLinePlacement(
				sc.linePlacement.GetOffset(),
				StringToLinePlacementMode(sc.linePlacement.GetLinePlacementMode()),
				sc.linePlacement.IsVisibleParts());
		}

		// areaPlacement
		if (sc.areaPlacement.isPresent())
		{
			symbol->SetAreaPlacement(StringToS100AreaPlacementMode(sc.areaPlacement.GetAreaPlacementMode()));
		}

		// overrideAll
	}
	// AugmentedPoint
	else if (sc.augmentedPoint.isPresent())
	{
		//sc.augmentedPoint
	}

	return (S100_PointInstruction*)instruction;
}

S100_LineInstruction* CommandToInstruction::ToS100LineInstruction(Local_DrawingCommands& dc, Local_StateCommands& sc)
{
	S100_Instruction* instruction = nullptr;
	if (sc.augmentedRay.isPresent())
	{
		instruction = new S100_AugmentedRay();
	}
	auto lineInstruction = new S100_LineInstruction();

	// DrawingInstruction
	SetDrawingInstruction(sc, lineInstruction);

	// PointInstruction
	if (dc.lineInstruction.isPresent())
	{
		
	}

	return lineInstruction;
}

bool CommandToInstruction::SetDrawingInstruction(Local_StateCommands& sc, S100_Instruction* out)
{
	if (out)
	{
		// id
		if (sc.id.isPresent())
		{
			out->setId(sc.id.GetId());
		}

		// parentId
		if (sc.parent.isPresent())
		{
			out->setParentId(sc.parent.GetParentId());
		}

		// hover
		if (sc.hover.isPresent())
		{
			out->setHover(sc.hover.GetHover());
		}

		// viewingGroup
		if (sc.viewingGroup.isPresent())
		{
			out->SetViewingGroup(sc.viewingGroup.GetViewingGroups());
		}

		// displayPlane
		if (sc.displayPlane.isPresent())
		{
			out->SetDisplayPlane(sc.displayPlane.GetDisplayPlane());
		}

		// drawingPriority
		if (sc.drawingPriority.isPresent())
		{
			out->SetDrawingPriority(sc.drawingPriority.GetDrawingPriority());
		}

		// scaleMinimum
		if (sc.scaleMinimum.isPresent())
		{
			out->SetScaleMinimum(sc.scaleMinimum.GetScaleMinimum());
		}

		// scaleMaximum
		if (sc.scaleMaximum.isPresent())
		{
			out->SetScaleMaximum(sc.scaleMaximum.GetScaleMaximum());
		}

		return true;
	}

	return false;
}

