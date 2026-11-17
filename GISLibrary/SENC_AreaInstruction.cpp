#include "stdafx.h"
#include "SENC_AreaInstruction.h"
#include "SENC_ColorFill.h"
#include "SENC_AreaFillReference.h"
#include "SENC_pixmapFill.h"
#include "SENC_SymbolFill.h"
#include "SENC_HatchFill.h"
#include "SENC_CommonFuc.h"
#include "PCOutputSchemaManager.h"


#include "../PortrayalCatalogue/PortrayalCatalogue.h"
#include "../PortrayalCatalogue/S100_ColorFill.h"
#include "../PortrayalCatalogue/S100_AreaInstruction.h"
#include "../PortrayalCatalogue/S100_AreaFillReference.h"

#include "../GISLibrary/R_FeatureRecord.h"

#include "SCommonFuction.h"
#include "SCoverage.h"

SENC_AreaInstruction::SENC_AreaInstruction()
{
	type = 3;
	areaFill = NULL;
}

SENC_AreaInstruction::~SENC_AreaInstruction()
{
	if (areaFill)
	{
		switch (areaFill->type)
		{
		case 1:
			delete (SENC_ColorFill*)areaFill;
			break;
		case 2:
			delete (SENC_AreaFillReference*)areaFill;
			break;
		case 3:
			delete (SENC_PixmapFill*)areaFill;
			break;
		case 4:
			delete (SENC_SymbolFill*)areaFill;
			break;
		case 5:
			delete (SENC_HatchFill*)areaFill;
			break;
		}
	}
}

void SENC_AreaInstruction::SetAreaFill(SENC_AreaFillBase* value)
{
	areaFill = value;
}


SENC_AreaFillBase* SENC_AreaInstruction::GetAreaFill()
{
	return areaFill;
}

void SENC_AreaInstruction::FromS100Instruction(
	S100_Instruction* s100Instruction,
	PortrayalCatalogue* /*pc*/,
	PCOutputSchemaManager* output)
{
	output->GetSENCFromS100Common(s100Instruction, this);

	SENC_ColorFill *colorFill = NULL;
	SENC_SymbolFill *symbolFill = NULL;

	auto s100AreaInstruction = (S100_AreaInstruction*)s100Instruction;

	if (s100AreaInstruction->GetAreaFill())
	{
		switch (s100AreaInstruction->GetAreaFill()->GetType())
		{
		case 1: // Color Fill
			areaFill = new SENC_ColorFill();

			colorFill = (SENC_ColorFill*)areaFill;
			if (((S100_ColorFill*)s100AreaInstruction->GetAreaFill())->GetColor())
			{
				colorFill->color = new SENC_Color();
				colorFill->color->SetToken(((S100_ColorFill*)s100AreaInstruction->GetAreaFill())->GetColor()->GetTokenW());
				colorFill->color->SetTransparency((float)std::wcstod(((S100_ColorFill*)s100AreaInstruction->GetAreaFill())->GetColor()->GetTransparencyW().c_str(), nullptr));
			}
			break;
		case 2:
		{
			areaFill = new SENC_AreaFillReference();
			auto s100AreaFillReference = (S100_AreaFillReference*)s100AreaInstruction->GetAreaFill();
			areaFill->SetFileStem(s100AreaFillReference->GetReferenceW());
		}
		break;
		case 4: // Symbol Fill
			areaFill = new SENC_SymbolFill();
			symbolFill = (SENC_SymbolFill*)areaFill;
			auto s100SymbolFill = (S100_SymbolFill*)s100AreaInstruction->GetAreaFill();

			symbolFill->areaCRSType = SENC_CommonFuc::GetAreaCRSType(s100SymbolFill->GetAreaCRSW());

			areaFill->SetFileStem(s100SymbolFill->GetFileStemW());

			auto s100Symbol = s100SymbolFill->GetSymbol();
			if (s100Symbol)
			{
				symbolFill->symbol = new SENC_Symbol();
				symbolFill->symbol->reference = s100Symbol->GetReferenceW();
				if (s100Symbol->GetRotation())
				{
					symbolFill->symbol->rotation = s100Symbol->GetRotation();
				}
				symbolFill->symbol->rotationCRS = SENC_CommonFuc::GetRotationCRS(s100Symbol->GetRotationCRS());
				symbolFill->symbol->scaleFactor = s100Symbol->GetScaleFactor();


				symbolFill->symbol->offset.x = s100Symbol->GetOffsetX();
				symbolFill->symbol->offset.y = s100Symbol->GetOffsetY();
			}

			if (((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetV1())
			{
				symbolFill->v1.x = ((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetV1()->GetX();
				symbolFill->v1.y = ((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetV1()->GetY();
			}

			if (((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetV2())
			{
				symbolFill->v2.x = ((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetV2()->GetX();
				symbolFill->v2.y = ((S100_SymbolFill*)s100AreaInstruction->GetAreaFill())->GetV2()->GetY();
			}
		}
	}
}
