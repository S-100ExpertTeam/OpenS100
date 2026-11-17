#include "stdafx.h"
#include "SENC_CompositeLineStyle.h"
#include "SENC_LineStyle.h"
#include "SENC_LineStyleReference.h"

#include "../PortrayalCatalogue/S100_CompositeLineStyle.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"

SENC_CompositeLineStyle::SENC_CompositeLineStyle()
{
	type = 3;
}

SENC_CompositeLineStyle::~SENC_CompositeLineStyle()
{

	for (auto itor = lineStyles.begin(); itor != lineStyles.end(); itor++)
	{
		SENC_LineStyleBase *base = *itor;
		switch (base->type)
		{
		case 1:
			delete ((SENC_LineStyle*)base);
			break;
		case 2:
			delete ((SENC_LineStyleReference*)base);
			break;
		case 3:
			delete ((SENC_CompositeLineStyle*)base);
			break;
		}
	}
}

void SENC_CompositeLineStyle::GetStyleFromS100(S100_CompositeLineStyle* lineStyleS100, PortrayalCatalogue *pc, std::vector<SENC_LineStyle*> &_lineStyles)
{
	auto lineStyle = lineStyleS100->GetLineStyles();
	for (auto itor = lineStyle.begin(); itor != lineStyle.end(); itor++)
	{
		S100_LineStyleBase *base = *itor;
		SENC_LineStyleBase *sencBase = NULL;
		if (base->GetType() == 1)
		{
			sencBase = new SENC_LineStyle();
			SENC_LineStyle* ls = (SENC_LineStyle*)sencBase;
			ls->GetStyleFromS100((S100_LineStyle*)base, pc);
		}
		else if (base->GetType() == 2)
		{
			sencBase = new SENC_LineStyleReference();
			SENC_LineStyleReference* ls = (SENC_LineStyleReference*)sencBase;

			ls->GetStyleFromS100((S100_LineStyleReference*) base);
		}
		else if (base->GetType() == 3)
		{
			sencBase = new SENC_CompositeLineStyle();
			SENC_CompositeLineStyle* ls = (SENC_CompositeLineStyle*)sencBase;
			ls->GetStyleFromS100((S100_CompositeLineStyle*)base, pc, _lineStyles);
		}
	}
}
