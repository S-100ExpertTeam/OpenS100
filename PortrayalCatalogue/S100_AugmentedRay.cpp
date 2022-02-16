#include "stdafx.h"
#include "S100_AugmentedRay.h"
#include "PortrayalCatalogue.h"

S100_AugmentedRay::S100_AugmentedRay()
{
	SetLineStyle(NULL);
	SetType(7);
}

S100_AugmentedRay::~S100_AugmentedRay()
{

}

void S100_AugmentedRay::GetContents(pugi::xml_node node)
{
	for (auto instruction=node.first_child(); instruction; instruction=instruction)
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
			if (!GetCompositeLineStyle()) SetCompositeLineStyle(new S100_CompositeLineStyle());
			GetCompositeLineStyle()->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "direction"))
		{
			direction = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "length"))
		{
			length = pugi::as_wide(instruction.child_value());
		}
		else if (!strcmp(instructionName, "crs"))
		{
			SetCrsType(pugi::as_wide(instruction.child_value()));
		}
	}
}

void S100_AugmentedRay::SetDirection(std::wstring& value)
{
	direction = value;
}

void S100_AugmentedRay::SetLength(std::wstring& value)
{
	length = value;
}

std::wstring S100_AugmentedRay::GetDirection()
{
	return direction;
}

std::wstring S100_AugmentedRay::GetLength() 
{
	return length;
}