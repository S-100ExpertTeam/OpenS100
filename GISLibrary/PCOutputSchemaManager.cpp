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

#include "..\\S100_SVG_D2D1_DLL\\UnitOfCSS.h"
#include "..\\S100_SVG_D2D1_DLL\\CSSStyle.h"
#include "..\\S100_SVG_D2D1_DLL\\CSSStroke.h"
#include "..\\S100_SVG_D2D1_DLL\\CSS.h"

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
				sit->fr->m_alertIndicationType = sit->alertType;

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
	//S100_DisplayList *displayList;
	// to
	//SENC_DisplayList *displayListSENC;

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

		sit = SENC_Instruction::S1002SENC(it, pc, this);

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

				if (sit->drawingPriority_initValue != 0)
				{
					sit->fr->m_displayPriority =
						sit->drawingPriority_initValue < sit->fr->m_displayPriority
						? sit->drawingPriority_initValue : sit->fr->m_displayPriority;
				}
			}
		}
	}
}

SENC_NullInstruction* PCOutputSchemaManager::GetSENCFromS100(S100_NullInstruction* it, PortrayalCatalogue* pc) 
{
	SENC_NullInstruction* sit = new SENC_NullInstruction();
	GetSENCFromS100Common(it, sit);

	return sit;
}

SENC_PointInstruction* PCOutputSchemaManager::GetSENCFromS100(S100_PointInstruction* it, PortrayalCatalogue* pc)
{
	SENC_PointInstruction* sit = new SENC_PointInstruction();

	GetSENCFromS100Common(it, sit);
	if (it->GetSymbol())
	{
		sit->symbol = new SENC_Symbol();

		SENC_Symbol* s = sit->symbol;

		if (it->GetSymbol()->GetRotation())
		{
			s->rotation = it->GetSymbol()->GetRotation();
		}
		s->rotationCRS = SENC_CommonFuc::GetRotationCRS(it->GetSymbol()->GetRotationCRS());

		if (it->GetSymbol()->GetScaleFactor().size() == 0)
		{
			s->scaleFactor = 1.0;
		}
		else
		{
			s->scaleFactor = (float)(_wtof(it->GetSymbol()->GetScaleFactor().c_str()));
		}

		auto svgSymbolManagersvgmap = pc->GetSVGManager()->m_svgMap;

		auto itor = svgSymbolManagersvgmap.find(it->GetSymbol()->GetReference());
		if (itor != svgSymbolManagersvgmap.end())
		{
			sit->symbol->pSvg = itor->second;
		}
		else
		{
			itor = svgSymbolManagersvgmap.find(L"QUESMRK1");
			if (itor != svgSymbolManagersvgmap.end())
			{
				sit->symbol->pSvg = itor->second;
			}
		}

		if (it->GetVectorPoint())
		{
			if (!sit->vectorPoint) sit->vectorPoint = new SENC_VectorPoint();

			sit->vectorPoint->x = it->GetVectorPoint()->GetX();
			sit->vectorPoint->y = it->GetVectorPoint()->GetY();
		}
	}

	return sit;
}

SENC_LineInstruction* PCOutputSchemaManager::GetSENCFromS100(S100_LineInstruction* it, PortrayalCatalogue* pc)
{
	SENC_LineInstruction* sit = new SENC_LineInstruction();
	GetSENCFromS100Common(it, sit);

	sit->SetSuppression(it->SuppressionIsTrue());

	if (it->GetCompositeLineStyle())
	{
		auto lineStyles = it->GetCompositeLineStyle()->GetLineStyles();
		for (auto itor = lineStyles.begin();
			itor != lineStyles.end();
			itor++)
		{
			SENC_LineStyle* ls = new SENC_LineStyle();
			ls->GetStyleFromS100(*itor, pc);

			for (auto itorSymbol = ls->symbols.begin(); itorSymbol != ls->symbols.end(); itorSymbol++)
			{
				SENC_LineSymbol* symbol = *itorSymbol;
				auto svgSymbolManagersvgmap = pc->GetSVGManager()->m_svgMap;
				auto itor = svgSymbolManagersvgmap.find(symbol->reference);
				if (itor != svgSymbolManagersvgmap.end())
				{
					symbol->pSvg = itor->second;
				}
				else
				{
					itor = svgSymbolManagersvgmap.find(L"QUESMRK1");
					if (itor != svgSymbolManagersvgmap.end())
					{
						symbol->pSvg = itor->second;
					}
				}
			}
			sit->lineStyles.push_back(ls);
		}
	}
	if (it->GetLineStyleReference())
	{
		auto lineStyles = pc->GetLineStyles();
		auto itorStyle = lineStyles.find(it->GetLineStyleReference()->GetReference());
		if (itorStyle == lineStyles.end())
		{

		}
		else
		{
			S100_LineStyleBase *base = itorStyle->second;
			if (base->GetType() == 1)
			{
				SENC_LineStyle* ls = new SENC_LineStyle();
				ls->SetLineStyleName(it->GetLineStyleReference()->GetReference());
				if (itorStyle != lineStyles.end())
				{
					ls->GetStyleFromS100((S100_LineStyle*)itorStyle->second, pc);

					for (auto itorSymbol = ls->symbols.begin(); itorSymbol != ls->symbols.end(); itorSymbol++)
					{
						SENC_LineSymbol* symbol = *itorSymbol;
						auto svgSymbolManger = pc->GetSVGManager();
						auto itor = svgSymbolManger->m_svgMap.find(symbol->reference);
						if (itor != svgSymbolManger->m_svgMap.end())
						{
							symbol->pSvg = itor->second;
						}
						else
						{
							itor = svgSymbolManger->m_svgMap.find(L"QUESMRK1");
							if (itor != svgSymbolManger->m_svgMap.end())
							{
								symbol->pSvg = itor->second;
							}
						}
					}
					sit->lineStyles.push_back(ls);
				}
			}

			else if (base->GetType() == 3) {
				S100_CompositeLineStyle* cls = (S100_CompositeLineStyle*)base;
				auto lineStyles = cls->GetLineStyles();
				for (auto itor = lineStyles.begin();
					itor != lineStyles.end();
					itor++)
				{
					S100_LineStyle* ls = *itor;
					SENC_LineStyle* sencls = new SENC_LineStyle();

					sencls->GetStyleFromS100(ls, pc);

					for (auto itorSymbol = sencls->symbols.begin(); itorSymbol != sencls->symbols.end(); itorSymbol++)
					{
						SENC_LineSymbol* symbol = *itorSymbol;
						auto svgSymbolManager = pc->GetSVGManager();
						auto itor = svgSymbolManager->m_svgMap.find(symbol->reference);
						if (itor != svgSymbolManager->m_svgMap.end())
						{
							symbol->pSvg = itor->second;
						}
						else
						{
							itor = svgSymbolManager->m_svgMap.find(L"QUESMRK1");
							if (itor != svgSymbolManager->m_svgMap.end())
							{
								symbol->pSvg = itor->second;
							}
						}
					}
					sit->lineStyles.push_back(sencls);
				}
			}
		}
	}
	if (it->GetLineStyle())
	{
		SENC_LineStyle* ls = new SENC_LineStyle();
		ls->GetStyleFromS100(it->GetLineStyle(), pc);

		for (auto itorSymbol = ls->symbols.begin(); itorSymbol != ls->symbols.end(); itorSymbol++)
		{
			SENC_LineSymbol* symbol = *itorSymbol;
			auto svgSymbolManager = pc->GetSVGManager();
			auto itor = svgSymbolManager->m_svgMap.find(symbol->reference);
			if (itor != svgSymbolManager->m_svgMap.end())
			{
				symbol->pSvg = itor->second;
			}
			else
			{
				itor = svgSymbolManager->m_svgMap.find(L"QUESMRK1");
				if (itor != svgSymbolManager->m_svgMap.end())
				{
					symbol->pSvg = itor->second;
				}
			}
		}
		sit->lineStyles.push_back(ls);
	}

	return sit;
}

#pragma warning(disable:4244)
SENC_AreaInstruction* PCOutputSchemaManager::GetSENCFromS100(S100_AreaInstruction* it, PortrayalCatalogue* pc)
{
	SENC_AreaInstruction* sit = new SENC_AreaInstruction();
	GetSENCFromS100Common(it, sit);

	SENC_ColorFill *colorFill = NULL;
	SENC_SymbolFill *symbolFill = NULL;
	SENC_AreaFillReference *areaFillReference = NULL;
	SENC_PixmapFill *pixmapFill = NULL;
	SENC_HatchFill *hatchFill = NULL;

	if (it->GetAreaFill())
	{
		switch (it->GetAreaFill()->GetType())
		{
		case 1: // Color Fill
			sit->areaFill = new SENC_ColorFill();

			colorFill = (SENC_ColorFill*)sit->areaFill;
			if (((S100_ColorFill*)it->GetAreaFill())->GetColor())
			{
				colorFill->color = new SENC_Color();
				colorFill->color->token = ((S100_ColorFill*)it->GetAreaFill())->GetColor()->GetToken();
				colorFill->color->transparency = _wtof(((S100_ColorFill*)it->GetAreaFill())->GetColor()->GetTransparency().c_str());

				auto colorProfile = pc->GetColorProfile();
				if (colorProfile)
				{
					colorFill->color->RGBColor = colorProfile->GetRGBRef(pc->GetCurrentPaletteName(), colorFill->color->token);;
				}
			}
			break;
		case 2:
		{
			sit->areaFill = new SENC_AreaFillReference();
			auto s100AreaFillReference = (S100_AreaFillReference*)it->GetAreaFill();
			sit->areaFill->SetFileTitle(s100AreaFillReference->GetReference());
		}
		break;
		case 4: // Symbol Fill
			sit->areaFill = new SENC_SymbolFill();
			symbolFill = (SENC_SymbolFill*)sit->areaFill;
			symbolFill->areaCRSType = SENC_CommonFuc::GetAreaCRSType(((S100_SymbolFill*)it->GetAreaFill())->GetAreaCRS());

			sit->areaFill->SetFileTitle(it->GetAreaFill()->GetFileTitle());

			if (((S100_SymbolFill*)it->GetAreaFill())->GetSymbol())
			{
				symbolFill->symbol = new SENC_Symbol();
				symbolFill->symbol->reference = ((S100_SymbolFill*)it->GetAreaFill())->GetSymbol()->GetReference();
				if (((S100_SymbolFill*)it->GetAreaFill())->GetSymbol()->GetRotation())
				{
					symbolFill->symbol->rotation = ((S100_SymbolFill*)it->GetAreaFill())->GetSymbol()->GetRotation();
				}
				symbolFill->symbol->rotationCRS = SENC_CommonFuc::GetRotationCRS(((S100_SymbolFill*)it->GetAreaFill())->GetSymbol()->GetRotationCRS());
				symbolFill->symbol->scaleFactor = (float)(_wtof(((S100_SymbolFill*)it->GetAreaFill())->GetSymbol()->GetScaleFactor().c_str()));

				auto svgSymbolManager = pc->GetSVGManager();
				auto itor = svgSymbolManager->m_svgMap.find(symbolFill->symbol->reference);
				if (itor != svgSymbolManager->m_svgMap.end())
				{
					symbolFill->symbol->pSvg = itor->second;
				}
				else
				{
					itor = svgSymbolManager->m_svgMap.find(L"QUESMRK1");
					if (itor != svgSymbolManager->m_svgMap.end())
					{
						symbolFill->symbol->pSvg = itor->second;
					}
				}
			}
			if (((S100_SymbolFill*)it->GetAreaFill())->GetV1())
			{
				symbolFill->v1.x = ((S100_SymbolFill*)it->GetAreaFill())->GetV1()->GetX();
				symbolFill->v1.y = ((S100_SymbolFill*)it->GetAreaFill())->GetV1()->GetY();
			}
			if (((S100_SymbolFill*)it->GetAreaFill())->GetV2())
			{
				symbolFill->v2.x = ((S100_SymbolFill*)it->GetAreaFill())->GetV2()->GetX();
				symbolFill->v2.y = ((S100_SymbolFill*)it->GetAreaFill())->GetV2()->GetY();
			}
		}
	}
	return sit;
}
SENC_TextInstruction* PCOutputSchemaManager::GetSENCFromS100(S100_TextInstruction* it, PortrayalCatalogue* pc)
{
	SENC_TextInstruction* sit = new SENC_TextInstruction();
	GetSENCFromS100Common(it, sit);
	if (it->GetTextPoint())
	{
		sit->textPoint = new SENC_TextPoint();

		sit->textPoint->horizontalAlignment = SENC_CommonFuc::GetHorizontalAlignment(it->GetTextPoint()->GetHorizontalAlignment());
		sit->textPoint->verticalAlignment = SENC_CommonFuc::GetVerticalAlignment(it->GetTextPoint()->GetVerticalAlignment());
		if (it->GetTextPoint()->GetOffset())
		{
			sit->textPoint->offset.x = it->GetTextPoint()->GetOffset()->GetX();
			sit->textPoint->offset.y = it->GetTextPoint()->GetOffset()->GetY();
		}

		auto elements = it->GetTextPoint()->GetElemets();
		for (auto itor = elements.begin(); itor != elements.end(); itor++)
		{
			S100_Element* e1 = *itor;
			SENC_Element* e2 = new SENC_Element();

			sit->textPoint->elements.push_back(e2);

			e2->bodySize = _wtoi(e1->GetBodySize().c_str());
			e2->verticalOffset = (float)(_wtof(e1->GetVerticalOffset().c_str()));

			if (e1->GetFont())
			{
				e2->font.proportion = SENC_CommonFuc::GetFontProportion(e1->GetFont()->GetProportion());
				e2->font.serifs = _wtoi(e1->GetFont()->GetSerifs().c_str()) != 0 ? true : false;
				e2->font.slant = _wtoi(e1->GetFont()->GetSlant().c_str());
				e2->font.weight = _wtoi(e1->GetFont()->GetWeight().c_str());
			}

			if (e1->GetForground())
			{
				e2->foreground.token = e1->GetForground()->GetToken();
				e2->foreground.transparency = (int)(_wtof(e1->GetForground()->GetTransparency().c_str()) * 100);
			}
			if (e1->GetText())
			{
				e2->text.value = e1->GetText()->GetValue();
				e2->text.useValueOf = e1->GetText()->GetUseValueOf();
			}
		}
	}

	return sit;
}

SENC_AugmentedRay* PCOutputSchemaManager::GetSENCFromS100(S100_AugmentedRay* it, PortrayalCatalogue* pc)
{
	SENC_AugmentedRay* sit = new SENC_AugmentedRay();
	GetSENCFromS100Common(it, sit);

	sit->direction = _wtof(it->GetDirection().c_str());
	sit->length = _wtof(it->GetLength().c_str());

	sit->areaCRSType = SENC_CommonFuc::GetAreaCRSType(it->GetCrsType());

	if (it->GetCompositeLineStyle())
	{
		auto lineStyles = it->GetCompositeLineStyle()->GetLineStyles();
		for (auto itor = lineStyles.begin();
			itor != lineStyles.end();
			itor++)
		{
			SENC_LineStyle* ls = new SENC_LineStyle();
			ls->GetStyleFromS100(*itor, pc);

			for (auto itorSymbol = ls->symbols.begin(); itorSymbol != ls->symbols.end(); itorSymbol++)
			{
				SENC_LineSymbol* symbol = *itorSymbol;
				auto svgSymbolManager = pc->GetSVGManager();
				auto itor = svgSymbolManager->m_svgMap.find(symbol->reference);
				if (itor != svgSymbolManager->m_svgMap.end())
				{
					symbol->pSvg = itor->second;
				}
				else
				{
					itor = svgSymbolManager->m_svgMap.find(L"QUESMRK1");
					if (itor != svgSymbolManager->m_svgMap.end())
					{
						symbol->pSvg = itor->second;
					}
				}
			}
			sit->lineStyles.push_back(ls);
		}
	}
	if (it->GetLineStyleReference())
	{
		auto lineStyles = pc->GetLineStyles();
		auto itorStyle = lineStyles.find(it->GetLineStyleReference()->GetReference());

		S100_LineStyleBase *base = itorStyle->second;
		if (base->GetType() == 1)
		{
			SENC_LineStyle* ls = new SENC_LineStyle();
			if (itorStyle != lineStyles.end())
			{
				ls->GetStyleFromS100((S100_LineStyle*)itorStyle->second, pc);

				for (auto itorSymbol = ls->symbols.begin(); itorSymbol != ls->symbols.end(); itorSymbol++)
				{
					SENC_LineSymbol* symbol = *itorSymbol;
					auto svgSymbolManager = pc->GetSVGManager();
					auto itor = svgSymbolManager->m_svgMap.find(symbol->reference);
					if (itor != svgSymbolManager->m_svgMap.end())
					{
						symbol->pSvg = itor->second;
					}
					else
					{
						itor = svgSymbolManager->m_svgMap.find(L"QUESMRK1");
						if (itor != svgSymbolManager->m_svgMap.end())
						{
							symbol->pSvg = itor->second;
						}
					}
				}
				sit->lineStyles.push_back(ls);
			}
		}
		else if (base->GetType() == 3) {

			S100_CompositeLineStyle* cls = (S100_CompositeLineStyle*)base;
		
			auto lineStyles = cls->GetLineStyles();
			for (auto itor = lineStyles.begin();
				itor != lineStyles.end();
				itor++)
			{
				S100_LineStyle* ls = *itor;
				SENC_LineStyle* sencls = new SENC_LineStyle();

				sencls->GetStyleFromS100(ls, pc);

				for (auto itorSymbol = sencls->symbols.begin(); itorSymbol != sencls->symbols.end(); itorSymbol++)
				{
					SENC_LineSymbol* symbol = *itorSymbol;
					auto svgSymbolManager = pc->GetSVGManager();
					auto itor = svgSymbolManager->m_svgMap.find(symbol->reference);
					if (itor != svgSymbolManager->m_svgMap.end())
					{
						symbol->pSvg = itor->second;
					}
					else
					{
						itor = svgSymbolManager->m_svgMap.find(L"QUESMRK1");
						if (itor != svgSymbolManager->m_svgMap.end())
						{
							symbol->pSvg = itor->second;
						}
					}
				}
				sit->lineStyles.push_back(sencls);
			}
		}
	}
	if (it->GetLineStyle())
	{
		SENC_LineStyle* ls = new SENC_LineStyle();
		ls->GetStyleFromS100(it->GetLineStyle(), pc);

		for (auto itorSymbol = ls->symbols.begin(); itorSymbol != ls->symbols.end(); itorSymbol++)
		{
			SENC_LineSymbol* symbol = *itorSymbol;
			auto svgSymbolManager = pc->GetSVGManager();
			auto itor = svgSymbolManager->m_svgMap.find(symbol->reference);
			if (itor != svgSymbolManager->m_svgMap.end())
			{
				symbol->pSvg = itor->second;
			}
			else
			{
				itor = svgSymbolManager->m_svgMap.find(L"QUESMRK1");
				if (itor != svgSymbolManager->m_svgMap.end())
				{
					symbol->pSvg = itor->second;
				}
			}
		}
		sit->lineStyles.push_back(ls);
	}


	return sit;
}

SENC_AugmentedPath* PCOutputSchemaManager::GetSENCFromS100(S100_AugmentedPath* it, PortrayalCatalogue* pc)
{
	SENC_AugmentedPath* sit = new SENC_AugmentedPath();
	GetSENCFromS100Common(it, sit);

	sit->areaCRSType = SENC_CommonFuc::GetAreaCRSType(it->GetCrsType());

	if (it->GetPath())
	{
		sit->path = new SENC_Path();
		auto PolyLines = it->GetPath()->GetPolylines();
		for (auto itor = PolyLines.begin(); itor != PolyLines.end(); itor++)
		{

		}

		auto arc3point = it->GetPath()->GetArc3Pointses();
		for (auto itor = arc3point.begin(); itor != arc3point.end(); itor++)
		{

		}

		auto arcBy = it->GetPath()->GetArcByRadiuses();
		for (auto itor = arcBy.begin(); itor != arcBy.end(); itor++)
		{
			S100_ArcByRadius* p = &(*itor);
			SENC_ArcByRadius* sp = new SENC_ArcByRadius();
			sp->center.x = p->GetCenter()->GetX();
			sp->center.y = p->GetCenter()->GetY();
			sp->radius = _wtof(p->GetRadius().c_str());
			if (p->GetSector())
			{
				sp->sector = new SENC_Sector();
				sp->sector->angularDistance = _wtof(p->GetSector()->GetAnglearDistance().c_str());
				sp->sector->startAngle = _wtof(p->GetSector()->GetStartAngle().c_str());
			}
			sit->path->arcByRadiuses.push_back(sp);
		}

		auto annulu = it->GetPath()->GetAnnuluses();
		for (auto itor = annulu.begin(); itor != annulu.end(); itor++)
		{

		}
	}

	if (it->GetCompositeLineStyle())
	{
		auto lineStyles = it->GetCompositeLineStyle()->GetLineStyles();

		for (auto itor = lineStyles.begin();
			itor != lineStyles.end();
			itor++)
		{
			SENC_LineStyle* ls = new SENC_LineStyle();
			ls->GetStyleFromS100(*itor, pc);

			for (auto itorSymbol = ls->symbols.begin(); itorSymbol != ls->symbols.end(); itorSymbol++)
			{
				SENC_LineSymbol* symbol = *itorSymbol;
				auto svgSymbolManager = pc->GetSVGManager();
				auto itor = svgSymbolManager->m_svgMap.find(symbol->reference);
				if (itor != svgSymbolManager->m_svgMap.end())
				{
					symbol->pSvg = itor->second;
				}
				else
				{
					itor = svgSymbolManager->m_svgMap.find(L"QUESMRK1");
					if (itor != svgSymbolManager->m_svgMap.end())
					{
						symbol->pSvg = itor->second;
					}
				}
			}
			sit->lineStyles.push_back(ls);
		}
	}
	if (it->GetLineStyleReference())
	{
		auto lineStyles = pc->GetLineStyles();
		auto itorStyle = lineStyles.find(it->GetLineStyleReference()->GetReference());

		S100_LineStyleBase *base = itorStyle->second;
		if (base->GetType() == 1)
		{
			SENC_LineStyle* ls = new SENC_LineStyle();
			if (itorStyle != lineStyles.end())
			{
				ls->GetStyleFromS100((S100_LineStyle*)itorStyle->second, pc);

				for (auto itorSymbol = ls->symbols.begin(); itorSymbol != ls->symbols.end(); itorSymbol++)
				{
					SENC_LineSymbol* symbol = *itorSymbol;
					auto svgSymbolManager = pc->GetSVGManager();
					auto itor = svgSymbolManager->m_svgMap.find(symbol->reference);
					if (itor != svgSymbolManager->m_svgMap.end())
					{
						symbol->pSvg = itor->second;
					}
					else
					{
						itor = svgSymbolManager->m_svgMap.find(L"QUESMRK1");
						if (itor != svgSymbolManager->m_svgMap.end())
						{
							symbol->pSvg = itor->second;
						}
					}
				}
				sit->lineStyles.push_back(ls);
			}
		}
		else if (base->GetType() == 3) {
			S100_CompositeLineStyle* cls = (S100_CompositeLineStyle*)base;
			auto lineStyles = cls->GetLineStyles();
			for (auto itor = lineStyles.begin();
				itor != lineStyles.end();
				itor++)
			{
				S100_LineStyle* ls = *itor;
				SENC_LineStyle* sencls = new SENC_LineStyle();

				sencls->GetStyleFromS100(ls, pc);

				for (auto itorSymbol = sencls->symbols.begin(); itorSymbol != sencls->symbols.end(); itorSymbol++)
				{
					SENC_LineSymbol* symbol = *itorSymbol;
					auto svgSymbolManager = pc->GetSVGManager();
					auto itor = svgSymbolManager->m_svgMap.find(symbol->reference);
					if (itor != svgSymbolManager->m_svgMap.end())
					{
						symbol->pSvg = itor->second;
					}
					else
					{
						itor = svgSymbolManager->m_svgMap.find(L"QUESMRK1");
						if (itor != svgSymbolManager->m_svgMap.end())
						{
							symbol->pSvg = itor->second;
						}
					}
				}
				sit->lineStyles.push_back(sencls);
			}
		}
	}
	if (it->GetLineStyle())
	{
		SENC_LineStyle* ls = new SENC_LineStyle();
		ls->GetStyleFromS100(it->GetLineStyle(), pc);

		for (auto itorSymbol = ls->symbols.begin(); itorSymbol != ls->symbols.end(); itorSymbol++)
		{
			SENC_LineSymbol* symbol = *itorSymbol;
			auto svgSymbolManger = pc->GetSVGManager();
			auto itor = svgSymbolManger->m_svgMap.find(symbol->reference);
			if (itor != svgSymbolManger->m_svgMap.end())
			{
				symbol->pSvg = itor->second;
			}
			else
			{
				itor = svgSymbolManger->m_svgMap.find(L"QUESMRK1");
				if (itor != svgSymbolManger->m_svgMap.end())
				{
					symbol->pSvg = itor->second;
				}
			}
		}
		sit->lineStyles.push_back(ls);
	}
	return sit;
}

void PCOutputSchemaManager::GetSENCFromS100Common(S100_Instruction* tp, SENC_Instruction* si)
{
	
	si->displayPlane = tp->GetDisplayPlane().compare(L"UNDERRADAR") == 0 ? 0 : 1;
	si->drawingPriority = _wtoi(tp->GetDrawingProiority().c_str());
	si->drawingPriority_initValue = si->drawingPriority;
	si->viewingGroup = _wtoi(tp->GetViewingGroup().c_str());
	si->featureReference = _wtoi(tp->GetFeatureReference()->GetReference().c_str());
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