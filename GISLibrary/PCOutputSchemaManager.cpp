#include "stdafx.h"

#include "PCOutputSchemaManager.h"
#include "S101Cell.h"

#include "R_FeatureRecord.h"
#include "GISLibrary.h"
#include "S100Layer.h"
#include "SENC_CommonFuc.h"
#include "SENC_LineStyle.h"
#include "SENC_ColorFill.h"
#include "SENC_AreaFillReference.h"
#include "SENC_SymbolFill.h"
#include "SENC_PixmapFill.h"
#include "SENC_HatchFill.h"
#include "SENC_SpatialReference.h"
#include "SENC_DisplayList.h"
#include "SENC_Instruction.h"
#include "SENC_NullInstruction.h"
#include "SENC_PointInstruction.h"
#include "SENC_LineInstruction.h"
#include "SENC_AreaInstruction.h"
#include "SENC_TextInstruction.h"
#include "SENC_AugmentedRay.h"
#include "SENC_AugmentedPath.h"
#include "SENC_AlertReference.h"
#include "SENC_ArcByRadius.h"

#include "..\\PortrayalCatalogue\\S100_ColorFill.h"
#include "..\\PortrayalCatalogue\\S100_AreaFillReference.h"
#include "..\\PortrayalCatalogue\\PortrayalCatalogue.h"
#include "..\\PortrayalCatalogue\\S100_DisplayList.h"
#include "..\\PortrayalCatalogue\\S100_NullInstruction.h"
#include "..\\PortrayalCatalogue\\S100_PointInstruction.h"
#include "..\\PortrayalCatalogue\\S100_LineInstruction.h"
#include "..\\PortrayalCatalogue\\S100_AreaInstruction.h"
#include "..\\PortrayalCatalogue\\S100_TextInstruction.h"
#include "..\\PortrayalCatalogue\\S100_AugmentedPath.h"
#include "..\\PortrayalCatalogue\\S100_AugmentedRay.h"
#include "..\\PortrayalCatalogue\\S100_AlertReference.h"

#include "..\\S100Geometry\\SPoint.h"
#include "..\\S100Geometry\\SCoverage.h"


#include "..\\GeoMetryLibrary\\GeoCommonFuc.h"

#include <mmsystem.h> 

PCOutputSchemaManager::PCOutputSchemaManager()
{
	displayList = new S100_DisplayList();
	displayListSENC = new SENC_DisplayList();
}

PCOutputSchemaManager::~PCOutputSchemaManager()
{
	delete displayList;
	displayList = nullptr;

	delete displayListSENC;
	displayListSENC = nullptr;
}

void PCOutputSchemaManager::GenerateSENCAlertInstruction(S101Cell* cell, PortrayalCatalogue* pc)
{

	if (nullptr == displayListSENC)
	{
		displayListSENC = new SENC_DisplayList();
	}

	auto alertInstructions = displayList->GetAlertInstructions();

	for (auto itor = alertInstructions.begin();
		itor != alertInstructions.end();
		itor++)
	{
		S100_AlertReference* it = (S100_AlertReference*)*itor;
		SENC_AlertReference* sit = nullptr;
		sit = new SENC_AlertReference();

		GetSENCFromS100Common((S100_Instruction*)it, sit);

		if (it->alertType.size() > 0)
		{
			if (it->alertType.compare(L"ProhAre") == 0)
			{
				sit->alertType = 1;
			}
			else if (it->alertType.compare(L"SafetyContour") == 0)
			{
				sit->alertType = 2;
			}
			else if (it->alertType.compare(L"NavHazard") == 0)
			{
				sit->alertType = 3;
			}
		}
		sit->plan = it->plan;
		sit->monitor = it->monitor;

		if (sit)
		{
			__int64 iKey = ((__int64)100) << 32 | sit->featureReference;
			auto item = cell->GetFeatureRecord(iKey);

			if (nullptr == item)
			{
				continue;
			}
			else
			{
				sit->fr = item;
			}
			
			if (sit->fr)
			{
				sit->fr->m_alertIndicationType = sit->alertType;
			}

			displayListSENC->AddAlertIndication(sit);
		}
	}
}

void PCOutputSchemaManager::InitDisplayList()
{
	delete displayList;
	displayList = nullptr;
}

void PCOutputSchemaManager::GenerateSENCInstruction(S101Cell* cell, PortrayalCatalogue* pc)
{
	if (nullptr == displayListSENC)
	{
		displayListSENC = new SENC_DisplayList();
	}

	auto displayInstructions = displayList->GetDisplayInstructions();

	for (auto itor = displayInstructions.begin();
		itor != displayInstructions.end();
		itor++)
	{
		S100_Instruction* it = *itor;

		/*
		*  Type Of Instruction
		*  0 : Null Instruction
		*  1 : Point Instruction
		*  2 : Line Instruction
		*  3 : Area Instruction
		** 4 : Coverage Instruction
		*  5 : Text Instruction
		** 6 : Augmented Point
		*  7 : Augmented Ray
		*  8 : Augmented Path
		** 9 : Augmented Area
		*/
		SENC_Instruction* sit = nullptr;

		sit = SENC_Instruction::S1002SENC(it, pc, this, cell);

		if (sit)
		{
			displayListSENC->AddInstruction(sit);

			__int64 iKey = ((__int64)100) << 32 | sit->featureReference;
			auto item = cell->GetFeatureRecord(iKey);

			if (nullptr == item)
			{
				continue;
			}
			else
			{
				sit->fr = item;

				auto currentPriority = sit->DrawingPriority();
				if (currentPriority != 0)
				{
					if (currentPriority < sit->fr->MinimumDisplayPriority() || 
						sit->fr->MinimumDisplayPriority() == -1)
					{
						sit->fr->MinimumDisplayPriority(currentPriority);
					}

					if (currentPriority > sit->fr->MaximumDisplayPriority() ||
						sit->fr->MaximumDisplayPriority() == -1)
					{
						sit->fr->MaximumDisplayPriority(currentPriority);
					}
				}
			}
		}
	}
}

void PCOutputSchemaManager::GetSENCFromS100Common(S100_Instruction* tp, SENC_Instruction* si)
{
	si->displayPlane = tp->GetDisplayPlane().compare(L"UNDERRADAR") == 0 ? 0 : 1;
	si->drawingPriority = _wtoi(tp->GetDrawingProiority().c_str());
	si->viewingGroup = _wtoi(tp->GetViewingGroup().c_str());
	si->featureReference = _wtoi(tp->GetFeatureReference().c_str());
	si->scaleMinimum = _wtoi(tp->GetScaleMinimum().c_str());
	si->scaleMaximum = _wtoi(tp->GetScaleMaximum().c_str());

	auto spatialReference = tp->GetSpatialReference();
	for (auto itor = spatialReference.begin(); itor != spatialReference.end(); itor++)
	{
		S100_SpatialReference* sr = *itor;
		SENC_SpatialReference* ssr = new SENC_SpatialReference();

		ssr->SetRCNM(sr->GetType());
		ssr->reference = _wtoi(sr->GetReference().c_str());
		ssr->bForward = sr->GetForward().compare(L"true") == 0 ? true : false;

		si->spatialReference.push_back(ssr);
	}
}

void PCOutputSchemaManager::ChangePallete(PortrayalCatalogue *pc)
{
	if (displayListSENC)
	{
		displayListSENC->ChangePallete(pc);
	}
}