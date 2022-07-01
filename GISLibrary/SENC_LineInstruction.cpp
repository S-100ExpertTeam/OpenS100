#include "stdafx.h"
#include "SENC_LineInstruction.h"
#include "SENC_LineStyle.h"
#include "SENC_LineStyleReference.h"
#include "SENC_CompositeLineStyle.h"
#include "SENC_SpatialReference.h"
#include "PCOutputSchemaManager.h"

#include "..\\GISLibrary\\R_FeatureRecord.h"
#include "..\\S100Geometry\\SCommonFuction.h"
#include "..\\S100Geometry\\SCurveHasOrient.h"
#include "..\\S100Geometry\\SCurve.h"
#include "..\\GeoMetryLibrary\\GeoCommonFuc.h"
#include "..\\PortrayalCatalogue\\PortrayalCatalogue.h"
#include "..\\PortrayalCatalogue\\S100_LineInstruction.h"
#include "..\\S100Engine\\S100Render.h"

#include <ctime> 
#include <mmsystem.h> 

SENC_LineInstruction::SENC_LineInstruction()
{
	type = 2;
}

SENC_LineInstruction::~SENC_LineInstruction()
{	
	for (auto itor = lineStyles.begin(); itor != lineStyles.end(); itor++)
	{
		switch ((*itor)->type)
		{
		case 0:
			delete *itor;
			break;
		case 1:
			delete ((SENC_LineStyle*)*itor);
			break;
		case 2:
			delete ((SENC_LineStyleReference*)*itor);
			break;
		case 3:
			delete ((SENC_CompositeLineStyle*)*itor);
			break;
		}
	}
}

void SENC_LineInstruction::SetSuppression(bool value)
{
	suppression = value;
}

bool SENC_LineInstruction::GetSuppression()
{
	return suppression;
}

// Line Instruction
#pragma warning(disable:4244)
#pragma warning(disable:4838)
void SENC_LineInstruction::DrawInstruction(ID2D1DCRenderTarget* rt, ID2D1Factory1* pDirect2dFactory, ID2D1SolidColorBrush* brush, std::vector<ID2D1StrokeStyle1*>* strokeGroup, Scaler *scaler, PortrayalCatalogue* pc)
{
	std::list<SCurveHasOrient>* curListCurveLink = nullptr;

	// Surface
	if (fr->m_geometry->type == 3)
	{
		auto surface = (SSurface*)fr->m_geometry;

		curListCurveLink = &surface->curveList;
	}
	// Curve
	else if (fr->m_geometry->type == 2)
	{
		auto compositeCurve = (SCompositeCurve*)fr->m_geometry;

		curListCurveLink = &compositeCurve->m_listCurveLink;
	}

	if (nullptr == curListCurveLink)
	{
		OutputDebugString(L"The spatial information format of the object is invalid..");
		return;
	}

	for (auto i = curListCurveLink->begin(); i != curListCurveLink->end(); i++)
	{
		auto curve = (*i).GetCurve();

		if ((*i).GetIsDuplicated())
		{
			continue;
		}

		// Code for cases where Drawing Instructions has a Special Reference.
		auto compareCurve = curve;
		int compareCurveId = compareCurve->m_id & 0x0000FFFF;
		for (auto it = spatialReference.begin(); it != spatialReference.end(); it++)
		{
			curve = nullptr;
			unsigned ref = (*it)->reference;

			if (compareCurveId == ref)
			{
				curve = compareCurve;
				break;
			}
		}

		if (nullptr == curve)
		{
			//Case where there is a special reference, but the corresponding curve is not found during the reference.
			continue;
		}

		for (auto itor = lineStyles.begin(); itor != lineStyles.end(); itor++)
		{
			SENC_LineStyleBase *lineStyleBase = *itor;
			lineStyleBase->DrawInstruction(&*i, rt, pDirect2dFactory, brush, strokeGroup, scaler, pc);
		}
	}
}

void SENC_LineInstruction::ChangePallete(PortrayalCatalogue *pc)
{
	for (auto i = lineStyles.begin(); i != lineStyles.end(); i++)
	{
		(*i)->ChangePallete(pc);
	}
}

void SENC_LineInstruction::FromS100Instruction(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output)
{
	auto s100LineInstruction = (S100_LineInstruction*)s100Instruction;

	output->GetSENCFromS100Common(s100Instruction, this);

	SetSuppression(s100LineInstruction->SuppressionIsTrue());

	if (s100LineInstruction->GetCompositeLineStyle())
	{
		auto lineStylelist = s100LineInstruction->GetCompositeLineStyle()->GetLineStyles();
		for (auto itor = lineStylelist.begin();
			itor != lineStylelist.end();
			itor++)
		{
			SENC_LineStyle* ls = new SENC_LineStyle();
			ls->GetStyleFromS100(*itor, pc);
			lineStyles.push_back(ls);
		}
	}

	if (s100LineInstruction->GetLineStyleReference())
	{
		auto lineStyleList = pc->GetLineStyles();
		auto itorStyle = lineStyleList.find(s100LineInstruction->GetLineStyleReference()->GetReference());
		if (itorStyle == lineStyleList.end())
		{

		}
		else
		{
			S100_LineStyleBase *base = itorStyle->second;
			if (base->GetType() == 1)
			{
				SENC_LineStyle* ls = new SENC_LineStyle();
				ls->SetLineStyleName(s100LineInstruction->GetLineStyleReference()->GetReference());
				if (itorStyle != lineStyleList.end())
				{
					ls->GetStyleFromS100((S100_LineStyle*)itorStyle->second, pc);
					lineStyles.push_back(ls);
				}
			}
			else if (base->GetType() == 3) {
				S100_CompositeLineStyle* cls = (S100_CompositeLineStyle*)base;

				auto lineStyleslist = cls->GetLineStyles();
				for (auto itor = lineStyleslist.begin();
					itor != lineStyleslist.end();
					itor++)
				{
					S100_LineStyle* ls = *itor;
					SENC_LineStyle* sencls = new SENC_LineStyle();

					sencls->GetStyleFromS100(ls, pc);
					lineStyles.push_back(sencls);
				}
			}
		}
	}

	if (s100LineInstruction->GetLineStyle())
	{
		SENC_LineStyle* ls = new SENC_LineStyle();
		ls->GetStyleFromS100(s100LineInstruction->GetLineStyle(), pc);
		lineStyles.push_back(ls);
	}
}