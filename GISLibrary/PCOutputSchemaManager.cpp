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
#include "SPoint.h"
#include "SCoverage.h"

#include "../PortrayalCatalogue/PortrayalCatalogue.h"
#include "../PortrayalCatalogue/DisplayFactory.h"
/*
#include "../PortrayalCatalogue/S100_ColorFill.h"
#include "../PortrayalCatalogue/S100_AreaFillReference.h"
#include "../PortrayalCatalogue/S100_DisplayList.h"
#include "../PortrayalCatalogue/S100_NullInstruction.h"
#include "../PortrayalCatalogue/S100_PointInstruction.h"
#include "../PortrayalCatalogue/S100_LineInstruction.h"
#include "../PortrayalCatalogue/S100_AreaInstruction.h"
#include "../PortrayalCatalogue/S100_TextInstruction.h"
#include "../PortrayalCatalogue/S100_AugmentedPath.h"
#include "../PortrayalCatalogue/S100_AugmentedRay.h"
#include "../PortrayalCatalogue/S100_AlertReference.h"
*/

#include "../GeoMetryLibrary/GeoCommonFuc.h"

#include <mmsystem.h> 

PCOutputSchemaManager::PCOutputSchemaManager()
{
	displayList = CDisplayFactory::createDisplayList();
	displayListSENC = new SENC_DisplayList();
}

PCOutputSchemaManager::~PCOutputSchemaManager()
{
	CDisplayFactory::destroyDisplayList(displayList);
	displayList = nullptr;

	delete displayListSENC;
	displayListSENC = nullptr;
}

void PCOutputSchemaManager::InitDisplayList()
{
	CDisplayFactory::destroyDisplayList(displayList);
	displayList = nullptr;
}

void PCOutputSchemaManager::GenerateSENCInstruction(S100SpatialObject* s100so, PortrayalCatalogue* pc)
{
	if (!displayListSENC)
		displayListSENC = new SENC_DisplayList();

	for (const auto& iter : displayList->GetDisplayInstructions())
	{
		SENC_Instruction* sit = SENC_Instruction::S1002SENC(iter, pc, this, s100so);
		if (!sit)
			continue;

		auto featureID = pugi::as_utf8(iter->GetFeatureReference());
		auto featureType = s100so->GetFeatureType(featureID);
		if (!featureType)
			continue;

		displayListSENC->AddInstruction(sit);
		sit->fr = featureType;
	}
}

void PCOutputSchemaManager::GetSENCFromS100Common(S100_Instruction* tp, SENC_Instruction* si)
{
	si->displayPlane = tp->GetDisplayPlane().compare(L"UNDERRADAR") == 0 ? 0 : 1;
	si->drawingPriority = tp->GetDrawingProiority();
	si->viewingGroup = _wtoi(tp->GetViewingGroup(0).c_str());
	si->featureReference = pugi::as_utf8(tp->GetFeatureReference());
	si->scaleMinimum = tp->GetScaleMinimum();
	si->scaleMaximum = tp->GetScaleMaximum();

	auto spatialReference = tp->GetSpatialReference();
	for (auto itor = spatialReference.begin(); itor != spatialReference.end(); itor++)
	{
		S100_SpatialReference* sr = *itor;
		SENC_SpatialReference* ssr = new SENC_SpatialReference();

		ssr->SetRCNM(sr->GetType());
		ssr->reference = _wtoi(sr->GetReference().c_str());
		ssr->bForward = sr->isForward();

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
