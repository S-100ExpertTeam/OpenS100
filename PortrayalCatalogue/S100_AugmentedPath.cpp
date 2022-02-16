#include "stdafx.h"
#include "S100_AugmentedPath.h"
#include "PortrayalCatalogue.h"

S100_AugmentedPath::S100_AugmentedPath()
{
	SetType(8);
	path = NULL;
}

S100_AugmentedPath::~S100_AugmentedPath()
{
	if (path)
		delete path;
}

void S100_AugmentedPath::GetContents(pugi::xml_node node)
{

	for (auto instruction = node.first_child(); instruction; instruction = instruction)
	{
		auto instructionName = instruction.name();

		if (!strcmp(instructionName, "featureReference"))
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
		else if (!strcmp(instructionName, "path"))
		{
			if (!path) path = new S100_Path();
			path->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "lineStyle"))
		{
			if (!GetLineStyle()) SetLineStyle(new S100_LineStyle());
			GetLineStyle()->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "lineStyleReference"))
		{
			if (!GetLineStyleReference()) SetLineStyleReference(new S100_LineStyleReference());
			GetLineStyleReference()->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "compositeLineStyle"))
		{
			if (!GetCompositeLineStyle())SetCompositeLineStyle(new S100_CompositeLineStyle());
			GetCompositeLineStyle()->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "textPoint"))
		{
			if (!GetTextPoint()) SetTextPoint(new S100_TextPoint());
			GetTextPoint()->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "crs"))
		{
			SetCrsType(pugi::as_wide(instruction.child_value()));
		}
	}
}

void S100_AugmentedPath::SetPath(S100_Path* value)
{
	path = value;
}

S100_Path* S100_AugmentedPath::GetPath()
{
	return path;
}