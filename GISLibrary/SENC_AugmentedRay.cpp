#include "stdafx.h"
#include "SENC_AugmentedRay.h"
#include "SENC_LineStyle.h"
#include "SENC_LineStyleReference.h"
#include "SENC_CompositeLineStyle.h"
#include "SENC_CommonFuc.h"
#include "PCOutputSchemaManager.h"

#include "../GISLibrary/R_FeatureRecord.h"

#include "SCommonFuction.h"

#include "../GeoMetryLibrary/ENCCommon.h"
#include "../GeoMetryLibrary/GeoCommonFuc.h"

#include "../PortrayalCatalogue/S100_AugmentedRay.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"

#include "../LatLonUtility/LatLonUtility.h"

#include <ctime> 

SENC_AugmentedRay::SENC_AugmentedRay()
{
	type = 7;
}

SENC_AugmentedRay::~SENC_AugmentedRay()
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

#pragma warning(disable:4244)
#pragma warning(disable:4838)
void SENC_AugmentedRay::FromS100Instruction(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output)
{
	auto s100AugmentedRay = (S100_AugmentedRay*)s100Instruction;

	output->GetSENCFromS100Common(s100Instruction, this);

	direction = std::wcstod(s100AugmentedRay->GetDirectionW().c_str(), nullptr);
	length = std::wcstod(s100AugmentedRay->GetLengthW().c_str(), nullptr);

	areaCRSType = SENC_CommonFuc::GetAreaCRSType(s100AugmentedRay->GetCrsTypeW());

	if (s100AugmentedRay->GetCompositeLineStyle())
	{
		auto linestyleslist = s100AugmentedRay->GetCompositeLineStyle()->GetLineStyles();
		for (auto itor = linestyleslist.begin();
			itor != linestyleslist.end();
			itor++)
		{
			SENC_LineStyle* ls = new SENC_LineStyle();
			ls->GetStyleFromS100(*itor, pc);

			//for (auto itorSymbol = ls->symbols.begin(); itorSymbol != ls->symbols.end(); itorSymbol++)
			//{
			//	SENC_LineSymbol* symbol = *itorSymbol;
			//	auto svgsymbolManager = pc->GetSVGManager();
			//	auto itor = svgsymbolManager->m_svgMap.find(symbol->reference);
			//	if (itor != svgsymbolManager->m_svgMap.end())
			//	{
			//		symbol->pSvg = itor->second;
			//	}
			//	else
			//	{
			//		itor = svgsymbolManager->m_svgMap.find(L"QUESMRK1");
			//		if (itor != svgsymbolManager->m_svgMap.end())
			//		{
			//			symbol->pSvg = itor->second;
			//		}
			//	}
			//}
		
			lineStyles.push_back(ls);
		}
	}

	if (s100AugmentedRay->GetLineStyleReference())
	{
		auto lineStylelist = pc->GetLineStyles();
		auto itorStyle = lineStylelist.find(s100AugmentedRay->GetLineStyleReference()->GetReference());

		S100_LineStyleBase *base = itorStyle->second;
		if (base->GetType() == 1)
		{
			SENC_LineStyle* ls = new SENC_LineStyle();
			if (itorStyle != lineStylelist.end())
			{
				ls->GetStyleFromS100((S100_LineStyle*)itorStyle->second, pc);

				//for (auto itorSymbol = ls->symbols.begin(); itorSymbol != ls->symbols.end(); itorSymbol++)
				//{
				//	SENC_LineSymbol* symbol = *itorSymbol;
				//	auto svgsymbolManager = pc->GetSVGManager();
				//	auto itor = svgsymbolManager->m_svgMap.find(symbol->reference);
				//	if (itor != svgsymbolManager->m_svgMap.end())
				//	{
				//		symbol->pSvg = itor->second;
				//	}
				//	else
				//	{
				//		itor = svgsymbolManager->m_svgMap.find(L"QUESMRK1");
				//		if (itor != svgsymbolManager->m_svgMap.end())
				//		{
				//			symbol->pSvg = itor->second;
				//		}
				//	}
				//}
				lineStyles.push_back(ls);
			}
		}
		else if (base->GetType() == 3)
		{
			S100_CompositeLineStyle* cls = (S100_CompositeLineStyle*)base;
			auto lineStyleList = cls->GetLineStyles();
			for (auto itor = lineStyleList.begin();
				itor != lineStyleList.end();
				itor++)
			{
				S100_LineStyle* ls = *itor;
				SENC_LineStyle* sencls = new SENC_LineStyle();

				sencls->GetStyleFromS100(ls, pc);

				//for (auto itorSymbol = sencls->symbols.begin(); itorSymbol != sencls->symbols.end(); itorSymbol++)
				//{
				//	SENC_LineSymbol* symbol = *itorSymbol;
				//	auto svgsymbolManager = pc->GetSVGManager();
				//	auto itor = svgsymbolManager->m_svgMap.find(symbol->reference);
				//	if (itor != svgsymbolManager->m_svgMap.end())
				//	{
				//		symbol->pSvg = itor->second;
				//	}
				//	else
				//	{
				//		itor = svgsymbolManager->m_svgMap.find(L"QUESMRK1");
				//		if (itor != svgsymbolManager->m_svgMap.end())
				//		{
				//			symbol->pSvg = itor->second;
				//		}
				//	}
				//}
				lineStyles.push_back(sencls);
			}
		}
	}

	if (s100AugmentedRay->GetLineStyle())
	{
		SENC_LineStyle* ls = new SENC_LineStyle();
		ls->GetStyleFromS100(s100AugmentedRay->GetLineStyle(), pc);

		//for (auto itorSymbol = ls->symbols.begin(); itorSymbol != ls->symbols.end(); itorSymbol++)
		//{
		//	SENC_LineSymbol* symbol = *itorSymbol;
		//	auto svgsymbolManager = pc->GetSVGManager();
		//	auto itor = svgsymbolManager->m_svgMap.find(symbol->reference);
		//	if (itor != svgsymbolManager->m_svgMap.end())
		//	{
		//		symbol->pSvg = itor->second;
		//	}
		//	else
		//	{
		//		itor = svgsymbolManager->m_svgMap.find(L"QUESMRK1");
		//		if (itor != svgsymbolManager->m_svgMap.end())
		//		{
		//			symbol->pSvg = itor->second;
		//		}
		//	}
		//}

		lineStyles.push_back(ls);
	}
}
