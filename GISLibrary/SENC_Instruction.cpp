#include "stdafx.h"
#include "SENC_Instruction.h"
#include "SENC_SymbolFill.h"
#include "SENC_LineStyle.h"
#include "SENC_ArcByRadius.h"
#include "SENC_SpatialReference.h"
#include "SENC_NullInstruction.h"
#include "SENC_PointInstruction.h"
#include "SENC_LineInstruction.h"
#include "SENC_AreaInstruction.h"
#include "SENC_TextInstruction.h"
#include "SENC_AugmentedRay.h"
#include "SENC_AugmentedPath.h"
#include "R_FeatureRecord.h"
#include "S100SpatialObject.h"
#include "SSurface.h"

#include "../PortrayalCatalogue/S100_Symbol.h"
#include "../PortrayalCatalogue/PortrayalCatalogue.h"
#include "../PortrayalCatalogue/S100_NullInstruction.h"

#include "../GeoMetryLibrary/GeoPoint.h"

#include "../LatLonUtility/LatLonUtility.h"

SENC_Instruction::SENC_Instruction()
{
	featureReference = "";

	type = 0;
	suppressedInstance = false;
}

std::string SENC_Instruction::FeatureReference()
{
	return featureReference;
}

int SENC_Instruction::DrawingPriority()
{
	return drawingPriority;
}

SENC_Instruction::~SENC_Instruction()
{
	for (auto itor = spatialReference.begin(); itor != spatialReference.end(); itor++)
	{
		delete *itor;
	}
}

bool SENC_Instruction::HasSpatialReference()
{
	return spatialReference.size() > 0;
}

SENC_Instruction* SENC_Instruction::S1002SENC(S100_Instruction* s100Instruction, PortrayalCatalogue *pc, PCOutputSchemaManager* output, S100SpatialObject* so)
{
	SENC_Instruction *result = nullptr;

	switch (s100Instruction->GetType())
	{
	case 0: // NullInstruction
		result = new SENC_NullInstruction();
		break;
	case 1: // PointInstruction
		result = new SENC_PointInstruction();
		break;
	case 2: // LineInstruction
		result = new SENC_LineInstruction();
		break;
	case 3: // AreaInstruction
		result = new SENC_AreaInstruction();
		break;
	case 4: // CoverageInstruction
		break;
	case 5: // TextInstruction
		result = new SENC_TextInstruction();
		break;
	case 6: // AugmentedPoint
		break;
	case 7: // AugmentedRay
		result = new SENC_AugmentedRay();
		break;
	case 8: // AugmentedPath
		result = new SENC_AugmentedPath();
		break;
	case 9: // AugmentedArea
		break;
	}

	if (nullptr != result)
	{
		result->FromS100Instruction(s100Instruction, pc, output);
		//result->code = so->GetFeatureTypeCodeByID(result->featureReference);
	}
	return result;
}