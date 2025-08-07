#include "stdafx.h"
#include "CommandToInstruction.h"

S100_PointInstruction* CommandToInstruction::ToS100PointInstruction(Local_DrawingCommands& dc, Local_StateCommands& sc)
{
	auto pointInstruction = new S100_PointInstruction();

	// DrawingInstruction
	// id
	if (sc.id.isPresent())
	{
		pointInstruction->setId(sc.id.GetId());
	}

	// parentId
	if (sc.parent.isPresent())
	{
		pointInstruction->setParentId(sc.parent.GetParentId());
	}

	// hover
	if (sc.hover.isPresent())
	{
		pointInstruction->setHover(sc.hover.GetHover());
	}

	// viewingGroup
	if (sc.viewingGroup.isPresent())
	{
		pointInstruction->SetViewingGroup(sc.viewingGroup.GetViewingGroups());
	}

	// displayPlane
	if (sc.displayPlane.isPresent())
	{
		pointInstruction->SetDisplayPlane(sc.displayPlane.GetDisplayPlane());
	}

	// drawingPriority
	if (sc.drawingPriority.isPresent())
	{
		pointInstruction->SetDrawingPriority(sc.drawingPriority.GetDrawingPriority());
	}

	// scaleMinimum
	if (sc.scaleMinimum.isPresent())
	{
		pointInstruction->SetScaleMinimum(sc.scaleMinimum.GetScaleMinimum());
	}

	// scaleMaximum
	if (sc.scaleMaximum.isPresent())
	{
		pointInstruction->SetScaleMaximum(sc.scaleMaximum.GetScaleMaximum());
	}

	// PointInstruction
	if (dc.pointInstruction.isPresent())
	{
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

	return pointInstruction;
}
