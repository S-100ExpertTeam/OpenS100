#include "stdafx.h"
#include "S100_LineInstruction.h"
#include "PortrayalCatalogue.h"

S100_LineInstruction::S100_LineInstruction()
{
	SetType(2);

	lineStyleReference = NULL;
	compositeLineStyle = NULL;
	lineStyle = NULL;
}

S100_LineInstruction::~S100_LineInstruction()
{
	if (lineStyleReference)
	{
		delete lineStyleReference;
	}

	if (compositeLineStyle)
	{
		delete compositeLineStyle;
	}

	if (lineStyle)
	{
		delete lineStyle;
	}
}

void S100_LineInstruction::SetLineStyleReference(S100_LineStyleReference* value) { lineStyleReference = value; }
S100_LineStyleReference* S100_LineInstruction::GetLineStyleReference() { return lineStyleReference; }

void S100_LineInstruction::SetCompositeLineStyle(S100_CompositeLineStyle* value) { compositeLineStyle = value; }
S100_CompositeLineStyle* S100_LineInstruction::GetCompositeLineStyle() { return compositeLineStyle; }

void S100_LineInstruction::SetLineStyle(S100_LineStyle* value) { lineStyle = value; }
S100_LineStyle* S100_LineInstruction::GetLineStyle() { return lineStyle; }

void S100_LineInstruction::SetSuppression(const std::string& value)  { suppression = value; }
void S100_LineInstruction::SetSuppression(const std::wstring& value) { suppression = toUtf8(value); }
std::string  S100_LineInstruction::GetSuppression()  { return suppression; }
std::wstring S100_LineInstruction::GetSuppressionW() { return toWide(suppression); }

void S100_LineInstruction::GetContents(pugi::xml_node node)
{
	for (auto attri = node.first_attribute(); attri; attri = attri.next_attribute())
	{
		auto attriName = attri.name();
		if (!strcmp(attriName,"suppression"))
		{
			suppression = attri.value();
			break;
		}
	}

	for (auto instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
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
		else if (!strcmp(instructionName, "lineStyle"))
		{
			if (!lineStyle) {
				lineStyle = new S100_LineStyle();
			}
			lineStyle->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "lineStyleReference"))
		{
			if (!lineStyleReference) {
				lineStyleReference = new S100_LineStyleReference();
			}
			lineStyleReference->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "compositeLineStyle"))
		{
			if (!compositeLineStyle) compositeLineStyle = new S100_CompositeLineStyle();
			compositeLineStyle->GetContents(instruction);
		}
	}
}

bool S100_LineInstruction::SuppressionIsTrue()
{
	return suppression == "true";
}
