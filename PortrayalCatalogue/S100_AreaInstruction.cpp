#include "stdafx.h"
#include "S100_AreaInstruction.h"
#include "S100_ColorFill.h"
#include "S100_AreaFillReference.h"
#include "S100_pixmapFill.h"
#include "S100_SymbolFill.h"
#include "S100_HatchFill.h"

S100_AreaInstruction::S100_AreaInstruction()
{
	SetType(3);
	areaFill = NULL;
}

S100_AreaInstruction::~S100_AreaInstruction()
{	
	/*
	* 1 : ColorFill
	* 2 : AreaFillReference
	* 3 : PixmapFill
	* 4 : SymbolFill
	* 5 : HatchFill
	*/
	if (areaFill)
	{
		switch (areaFill->GetType())
		{
		case 1:
			delete (S100_ColorFill*)areaFill;
			break;
		case 2:
			delete (S100_AreaFillReference*)areaFill;
			break;
		case 3:
			delete (S100_PixmapFill*)areaFill;
			break;
		case 4:
			delete (S100_SymbolFill*)areaFill;
			break;
		case 5:
			delete (S100_HatchFill*)areaFill;
			break;

		}
	}
} 

void S100_AreaInstruction::GetContents(pugi::xml_node node)
{
	if (node==nullptr)
	{
		return;
	}

	for (auto instruction=node.first_child(); instruction; instruction= instruction.next_sibling())
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
		else if (!strcmp(instructionName, "symbolFill"))
		{
			if (!areaFill) areaFill = new S100_SymbolFill();
			((S100_SymbolFill*)areaFill)->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "colorFill"))
		{
			if (!areaFill) areaFill = new S100_ColorFill();
			((S100_ColorFill*)areaFill)->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "areaFillReference"))
		{
			if (!areaFill) areaFill = new S100_AreaFillReference();
			((S100_AreaFillReference*)areaFill)->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "pixmapFill"))
		{
			if (!areaFill) areaFill = new S100_PixmapFill();
			((S100_PixmapFill*)areaFill)->GetContents(instruction);
		}
		else if (!strcmp(instructionName, "hatchFill"))
		{
			if (!areaFill) areaFill = new S100_HatchFill();
			((S100_HatchFill*)areaFill)->GetContents(instruction);
		}

		if (areaFill)
		{
			if (areaFill->GetType() == 2)
			{
				S100_AreaFillReference *areaFillRef = (S100_AreaFillReference*)areaFill;
				std::wstring path = areaFillRef->GetReference();
				S100_SymbolFill* tempAreaFill = new S100_SymbolFill();

				if (!tempAreaFill->ReadFileByPugiXml(path + L".xml"))
				{
					delete tempAreaFill;
				}
				else
				{
					delete areaFillRef;
					areaFill = tempAreaFill;
				}
			}
		}
	}
}

void S100_AreaInstruction::SetAreaFill(S100_AreaFillBase* value) 
{
	areaFill = value;
}

S100_AreaFillBase* S100_AreaInstruction::GetAreaFill()
{
	return areaFill;
}