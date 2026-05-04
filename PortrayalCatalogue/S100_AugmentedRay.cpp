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
	for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		auto instructionName = instruction.name();

		if (!strcmp(instructionName, "featureReference"))
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
			direction = instruction.child_value();
		}
		else if (!strcmp(instructionName, "length"))
		{
			length = instruction.child_value();
		}
		else if (!strcmp(instructionName, "crs"))
		{
			SetCrsType(std::string(instruction.child_value()));
		}
	}
}

void S100_AugmentedRay::SetDirection(const std::string& value)  { direction = value; }
void S100_AugmentedRay::SetDirection(const std::wstring& value) { direction = toUtf8(value); }
std::string  S100_AugmentedRay::GetDirection()  { return direction; }
std::wstring S100_AugmentedRay::GetDirectionW() { return toWide(direction); }

void S100_AugmentedRay::SetLength(const std::string& value)  { length = value; }
void S100_AugmentedRay::SetLength(const std::wstring& value) { length = toUtf8(value); }
std::string  S100_AugmentedRay::GetLength()  { return length; }
std::wstring S100_AugmentedRay::GetLengthW() { return toWide(length); }
