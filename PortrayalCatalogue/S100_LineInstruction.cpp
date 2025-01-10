#include "stdafx.h"
#include "S100_LineInstruction.h"
#include "PortrayalCatalogue.h"

S100_LineInstruction::S100_LineInstruction()
{
	SetType(2);

	lineStyleReference = nullptr;
	compositeLineStyle = nullptr;
	lineStyle = nullptr;
}

S100_LineInstruction::~S100_LineInstruction()
{
	if (lineStyleReference)
		delete lineStyleReference, lineStyleReference = nullptr;

	if (compositeLineStyle)
		delete compositeLineStyle, compositeLineStyle = nullptr;

	if (lineStyle)
		delete lineStyle, lineStyle = nullptr;
}

void S100_LineInstruction::SetLineStyleReference(S100_LineStyleReference* value)
{
	lineStyleReference = value;
}

void S100_LineInstruction::SetLineStyleReference(std::wstring& reference)
{
	if (reference.empty())
		return;

	if (!lineStyleReference)
		lineStyleReference = new S100_LineStyleReference();

	lineStyleReference->SetReference(reference);
}

S100_LineStyleReference* S100_LineInstruction::GetLineStyleReference()
{
	return lineStyleReference;
}

void S100_LineInstruction::SetCompositeLineStyle(S100_CompositeLineStyle* value)
{
	compositeLineStyle = value;
}

S100_CompositeLineStyle* S100_LineInstruction::GetCompositeLineStyle()
{
	return compositeLineStyle;
}

void S100_LineInstruction::SetLineStyle(S100_LineStyle* value)
{
	lineStyle = value;
}

void S100_LineInstruction::SetLineStyle(std::string& value, std::string& dash)
{
	if (value.empty())
		return;

	if (!lineStyle)
		lineStyle = new S100_LineStyle();

	lineStyle->ParseValue(value);
	lineStyle->SetDash(dash);
}

S100_LineStyle* S100_LineInstruction::GetLineStyle()
{
	return lineStyle;
}

void S100_LineInstruction::SetSuppression(std::wstring& value)
{
	suppression = value;
}

std::wstring S100_LineInstruction::GetSuppression()
{
	return suppression;
}

void S100_LineInstruction::GetContents(pugi::xml_node node)
{
	//attri
	for (auto attri = node.first_attribute(); attri; attri = attri.next_attribute())
	{
		auto attriName = attri.name();

		if (!strcmp(attriName, "suppression"))
		{
			suppression = pugi::as_wide(attri.value());
			break;
		}
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
		else if (!strcmp(instructionName, "lineStyle"))
		{
			if (!lineStyle)
				lineStyle = new S100_LineStyle();
			lineStyle->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "lineStyleReference"))
		{
			if (!lineStyleReference)
				lineStyleReference = new S100_LineStyleReference();
			lineStyleReference->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "compositeLineStyle"))
		{
			if (!compositeLineStyle)
				compositeLineStyle = new S100_CompositeLineStyle();
			compositeLineStyle->GetContents(instruction);
		}
	}
}

bool S100_LineInstruction::SuppressionIsTrue()
{
	if (suppression.compare(L"true") == 0)
	{
		return true;
	}

	return false;
}