#include "stdafx.h"
#include "SENC_AugmentedPath.h"
#include "SENC_LineStyleReference.h"
#include "SENC_CompositeLineStyle.h"
#include "SENC_LineStyle.h"
#include "PCOutputSchemaManager.h"
#include "R_FeatureRecord.h"
#include "SENC_CommonFuc.h"
#include "SCommonFuction.h"

#include "../PortrayalCatalogue/S100_AugmentedPath.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"

SENC_AugmentedPath::SENC_AugmentedPath()
{
	path = NULL;
	type = 8;
}

SENC_AugmentedPath::~SENC_AugmentedPath()
{
	if (path)
		delete path;

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

#pragma warning(disable:4244)
void SENC_AugmentedPath::FromS100Instruction(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output)
{
	auto s100AugmentedPath = (S100_AugmentedPath*)s100Instruction;
	output->GetSENCFromS100Common(s100Instruction, this);

	areaCRSType = SENC_CommonFuc::GetAreaCRSType(s100AugmentedPath->GetCrsTypeW());

	if (s100AugmentedPath->GetPath())
	{
		path = new SENC_Path();
		auto poly = s100AugmentedPath->GetPath()->GetPolylines();
		for (auto itor = poly.begin(); itor != poly.end(); itor++)
		{

		}

		auto arc3 = s100AugmentedPath->GetPath()->GetArc3Pointses();
		for (auto itor = arc3.begin(); itor != arc3.end(); itor++)
		{

		}

		auto arcBy = s100AugmentedPath->GetPath()->GetArcByRadiuses();
		for (auto itor = arcBy.begin(); itor != arcBy.end(); itor++)
		{
			S100_ArcByRadius* p = &(*itor);
			SENC_ArcByRadius* sp = new SENC_ArcByRadius();
			sp->center.x = p->GetCenter()->GetX();
			sp->center.y = p->GetCenter()->GetY();
			sp->radius = std::wcstod(p->GetRadiusW().c_str(), nullptr);
			if (p->GetSector())
			{
				sp->sector = new SENC_Sector();
				sp->sector->angularDistance = std::wcstod(p->GetSector()->GetAnglearDistanceW().c_str(), nullptr);
				sp->sector->startAngle = std::wcstod(p->GetSector()->GetStartAngleW().c_str(), nullptr);
			}
			path->arcByRadiuses.push_back(sp);
		}
		auto annul = s100AugmentedPath->GetPath()->GetAnnuluses();
		for (auto itor = annul.begin(); itor != annul.end(); itor++)
		{

		}
	}

	if (s100AugmentedPath->GetCompositeLineStyle())
	{
		auto lineStyleslist = s100AugmentedPath->GetCompositeLineStyle()->GetLineStyles();
		for (auto itor = lineStyleslist.begin();
			itor != lineStyleslist.end();
			itor++)
		{
			SENC_LineStyle* ls = new SENC_LineStyle();
			ls->GetStyleFromS100(*itor, pc);
			lineStyles.push_back(ls);
		}
	}

	if (s100AugmentedPath->GetLineStyleReference())
	{
		auto lineStylelist = pc->GetLineStyles();
		auto itorStyle = lineStylelist.find(s100AugmentedPath->GetLineStyleReference()->GetReference());

		S100_LineStyleBase *base = itorStyle->second;
		if (base->GetType() == 1)
		{
			SENC_LineStyle* ls = new SENC_LineStyle();
			if (itorStyle != lineStylelist.end())
			{
				ls->GetStyleFromS100((S100_LineStyle*)itorStyle->second, pc);
				lineStyles.push_back(ls);
			}
		}
		else if (base->GetType() == 3) {

			S100_CompositeLineStyle* cls = (S100_CompositeLineStyle*)base;

			auto lineStylesList = cls->GetLineStyles();


			for (auto itor = lineStylesList.begin();
				itor != lineStylesList.end();
				itor++)
			{
				S100_LineStyle* ls = *itor;
				SENC_LineStyle* sencls = new SENC_LineStyle();

				sencls->GetStyleFromS100(ls, pc);
				lineStyles.push_back(sencls);
			}
		}
	}

	if (s100AugmentedPath->GetLineStyle())
	{
		SENC_LineStyle* ls = new SENC_LineStyle();
		ls->GetStyleFromS100(s100AugmentedPath->GetLineStyle(), pc);
		lineStyles.push_back(ls);
	}
}
