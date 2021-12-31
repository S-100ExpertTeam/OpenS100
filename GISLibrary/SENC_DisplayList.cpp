#include "stdafx.h"
#include "SENC_DisplayList.h"
#include "LayerManager.h"
#include "R_FeatureRecord.h"
#include "SENC_Instruction.h"
#include "SENC_NullInstruction.h"
#include "SENC_PointInstruction.h"
#include "SENC_LineInstruction.h"
#include "SENC_TextInstruction.h"
#include "SENC_AreaInstruction.h"
#include "SENC_AugmentedRay.h"
#include "SENC_AugmentedPath.h"
#include "SENC_AlertReference.h"

#include "..\\S100Geometry\\SGeometry.h"
#include "..\\GeoMetryLibrary\\Scaler.h"
#include "..\\GeoMetryLibrary\\ENCCommon.h"

SENC_DisplayList::SENC_DisplayList()
{

}

SENC_DisplayList::~SENC_DisplayList()
{
	for (auto itor = displayInstructions.begin(); itor != displayInstructions.end(); itor++)
	{
		SENC_Instruction* it = itor->second;

		/*
		*  Type Of Instruction
		*  0 : Null Instruction
		*  1 : Point Instruction
		*  2 : Line Instruction
		*  3 : Area Instruction
		** 4 : Coverage Instruction
		*  5 : Text Instruction
		** 6 : Augmented Point
		** 7 : Augmented Ray
		** 8 : Augmented Path
		** 9 : Augmented Area
		*/
		switch (it->type)
		{
		case 0: // Null Instruction
			delete (SENC_NullInstruction*)it;
			break;
		case 1: // Point Instruction
			delete (SENC_PointInstruction*)it;
			break;
		case 2: // Line Instruction
			delete (SENC_LineInstruction*)it;
			break;
		case 3: // Area Instruction
			delete (SENC_AreaInstruction*)it;
			break;
		case 5: // Text Instruction
			delete (SENC_TextInstruction*)it;
			break;
		case 7: // Augmented Ray Instruction
			delete (SENC_AugmentedRay*)it;
			break;
		case 8: // Text Instruction
			delete (SENC_AugmentedPath*)it;
			break;
		}
	}
	displayInstructions.clear();
	for (auto itor = alertInstructions.begin(); itor != alertInstructions.end(); itor++)
	{
		delete (SENC_AlertReference*)itor->second;
	}
	alertInstructions.clear();
}

void SENC_DisplayList::GetDrawingInstruction(int priority, int type, Scaler* scaler, std::list<SENC_Instruction*> &itList)
{
	auto currentScale = scaler->GetCurrentScale();
	int curViewingGroupLimit = 0;

	if (ENCCommon::DISPLAY_MODE == GeoMetryLibrary::DisplayModeTable::base)
	{
		curViewingGroupLimit = 19999;
	}
	else if (ENCCommon::DISPLAY_MODE == GeoMetryLibrary::DisplayModeTable::standard)
	{
		curViewingGroupLimit = 29999;
	}
	else if (ENCCommon::DISPLAY_MODE == GeoMetryLibrary::DisplayModeTable::all)
	{
		curViewingGroupLimit = 39999;
		curViewingGroupLimit = INT_MAX;
	}

	itList.clear();

	SENC_Instruction *instruction = nullptr;
	auto startIndex = displayInstructions.lower_bound(GetKey(priority, type));
	auto lastIndex = displayInstructions.upper_bound(GetKey(priority, type));

	for (auto i = startIndex;
		i != lastIndex;
		i++)
	{
		instruction = i->second;

		if (instruction->viewingGroup > curViewingGroupLimit)
		{
			continue;
		}

		if (false == instruction->suppressedInstance &&
			instruction->fr->m_geometry)
		{
			if (ENCCommon::APPLY_SCALE_MIN)
			{
				if ((instruction->fr->m_scaleMin == 0 || currentScale <= instruction->fr->m_scaleMin) &&
					(instruction->fr->m_scaleMax == 0 || currentScale >= instruction->fr->m_scaleMax) &&
					(MBR::CheckOverlap(scaler->GetMapCalcMBR(), instruction->fr->m_geometry->m_mbr)))
				{
					itList.push_back(instruction);
				}
			}
			else
			{
				if (MBR::CheckOverlap(scaler->GetMapCalcMBR(), instruction->fr->m_geometry->m_mbr))
				{
					itList.push_back(instruction);
				}
			}
		}
	}
}

void SENC_DisplayList::GetDrawingInstructionByCondition(int priority, int type, Scaler* scaler, std::list<SENC_Instruction*> &itList, int FeatureID_textPlacement)
{
	auto currentScale = scaler->GetCurrentScale();
	int curViewingGroupLimit = 0;

	if (ENCCommon::DISPLAY_MODE == GeoMetryLibrary::DisplayModeTable::base)
	{
		curViewingGroupLimit = 19999;
	}
	else if (ENCCommon::DISPLAY_MODE == GeoMetryLibrary::DisplayModeTable::standard)
	{
		curViewingGroupLimit = 29999;
	}
	else if (ENCCommon::DISPLAY_MODE == GeoMetryLibrary::DisplayModeTable::all)
	{
		curViewingGroupLimit = 39999;
	}

	itList.clear();

	for (auto i = displayInstructions.lower_bound(GetKey(priority, type)); i != displayInstructions.upper_bound(GetKey(priority, type)); i++)
	{
		auto instruction = i->second;

		if (!instruction->suppressedInstance && 
			instruction->fr->m_geometry)
		{
			if (instruction->viewingGroup > curViewingGroupLimit)
			{
				continue;
			}

			// [ Text Placement ]
			if (ENCCommon::SHOW_TEXT_PLACEMENT == TRUE)
			{
				if (instruction->fr->m_frid.m_nftc == FeatureID_textPlacement)
				{
					if (ENCCommon::APPLY_SCALE_MIN)
					{
						if (
							(instruction->fr->m_scaleMin == 0 || currentScale <= instruction->fr->m_scaleMin) &&
							(instruction->fr->m_scaleMax == 0 || currentScale >= instruction->fr->m_scaleMax) &&
							(MBR::CheckOverlap(scaler->GetMapCalcMBR(), instruction->fr->m_geometry->m_mbr)))
						{
							itList.push_back(instruction);
						}
					}
					else
					{
						if (MBR::CheckOverlap(scaler->GetMapCalcMBR(), instruction->fr->m_geometry->m_mbr))
						{
							itList.push_back(instruction);
						}
					}
				}
				else
				{
					if (ENCCommon::APPLY_SCALE_MIN)
					{
						if ((instruction->fr->m_scaleMin == 0 || currentScale <= instruction->fr->m_scaleMin) &&
							(instruction->fr->m_scaleMax == 0 || currentScale >= instruction->fr->m_scaleMax) &&
							(MBR::CheckOverlap(scaler->GetMapCalcMBR(), instruction->fr->m_geometry->m_mbr)))
						{
							if (instruction->fr->m_hasTextPlacement) 
							{
								continue;
							}
							else 
							{
								itList.push_back(instruction);
							}
						}
					}
					else
					{
						if (MBR::CheckOverlap(scaler->GetMapCalcMBR(), instruction->fr->m_geometry->m_mbr))
						{
							if (false == instruction->fr->m_hasTextPlacement) 
							{
								itList.push_back(instruction);
							}
						}
					}
				}
			}
			else
			{
				if (instruction->fr->m_frid.m_nftc == FeatureID_textPlacement)
				{
				}
				else
				{
					if (ENCCommon::APPLY_SCALE_MIN)
					{
						if ((instruction->fr->m_scaleMin == 0 || currentScale <= instruction->fr->m_scaleMin) &&
							(instruction->fr->m_scaleMax == 0 || currentScale >= instruction->fr->m_scaleMax) &&
							(MBR::CheckOverlap(scaler->GetMapCalcMBR(), instruction->fr->m_geometry->m_mbr)))
						{
							itList.push_back(instruction);
						}
					}
					else
					{
						if (MBR::CheckOverlap(scaler->GetMapCalcMBR(), instruction->fr->m_geometry->m_mbr))
						{
							itList.push_back(instruction);
						}
					}
				}
			}
		}
	}

	return;
}


void SENC_DisplayList::ChangePallete(PortrayalCatalogue *pc)
{
	for (auto i = displayInstructions.begin(); i != displayInstructions.end(); i++)
	{
		i->second->ChangePallete(pc);
	}
}

void SENC_DisplayList::AddInstruction(SENC_Instruction* value)
{
	/*0 : Null Instruction
		* 1 : Point Instruction
		* 2 : Line Instruction
		* 3 : Area Instruction
		** 4 : Coverage Instruction
		* 5 : Text Instruction
		** 6 : Augmented Point
		** 7 : Augmented Ray
		** 8 : Augmented Path
		** 9 : Augmented Area
		*/

	
	displayInstructions.insert({ GetKey(value->drawingPriority, value->type), value});
}

void SENC_DisplayList::AddAlertIndication(SENC_Instruction* value)
{
	/*0 : Null Instruction
	* 1 : Point Instruction
	* 2 : Line Instruction
	* 3 : Area Instruction
	** 4 : Coverage Instruction
	* 5 : Text Instruction
	** 6 : Augmented Point
	** 7 : Augmented Ray
	** 8 : Augmented Path
	** 9 : Augmented Area
	*/
	
	if (alertInstructions.find(value->fr->m_frid.m_name.GetName()) == alertInstructions.end())
	{
		alertInstructions.insert({ value->fr->m_frid.m_name.GetName(), (SENC_AlertReference*)value });
	}
	else
	{
		delete (SENC_AlertReference*)value;
	}
}

int SENC_DisplayList::GetType(int type)
{
	if (type == 3)
	{
		return 0;
	}
	else if (type == 2)
	{
		return 1;
	}
	else if (type == 1)
	{
		return 2;
	}
	else if (type == 7)
	{
		return 3;
	}
	else if (type == 8)
	{
		return 4;
	}
	else if (type == 5)
	{
		return 5;
	}

	return 10;
}

__int64 SENC_DisplayList::GetKey(int priority, int type)
{
	return ((__int64)priority) << 32 | GetType(type);
}