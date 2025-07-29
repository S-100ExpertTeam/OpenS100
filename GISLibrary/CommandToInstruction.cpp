#include "stdafx.h"
#include "CommandToInstruction.h"

S100_PointInstruction* CommandToInstruction::ToS100PointInstruction(Local_DrawingCommands& dc, Local_StateCommands& sc)
{
	auto pointInstruction = new S100_PointInstruction();

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

	return pointInstruction;
}
