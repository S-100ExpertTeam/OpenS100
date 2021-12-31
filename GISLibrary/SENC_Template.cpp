#include "stdafx.h"
#include "SENC_Template.h"
#include "SENC_NullInstruction.h"
#include "SENC_PointInstruction.h"
#include "SENC_LineInstruction.h"
#include "SENC_AreaInstruction.h"
#include "SENC_TextInstruction.h"
#include "SENC_AugmentedRay.h"
#include "SENC_AugmentedPath.h"
#include "SENC_CallTemplate.h"

SENC_Template::SENC_Template()
{
	priority = 1;
	callTemplate = NULL;
}

SENC_Template::~SENC_Template()
{
	for (auto itor = instructions.begin(); itor != instructions.end(); itor++)
	{
		SENC_Instruction *instruction = *itor;

		switch (instruction->type)
		{
		case 0:
			delete (SENC_NullInstruction*)instruction;
			break;
		case 1:
			delete (SENC_PointInstruction*)instruction;
			break;
		case 2:
			delete (SENC_LineInstruction*)instruction;
			break;
		case 3:
			delete (SENC_AreaInstruction*)instruction;
			break;
		case 4:
			delete (SENC_TextInstruction*)instruction;
			break;
		case 7:
			delete (SENC_AugmentedRay*)instruction;
			break;
		case 8:
			delete (SENC_AugmentedPath*)instruction;
			break;
		}
	}

	for (auto itor = matchAttr.begin(); itor != matchAttr.end(); itor++)
	{
		delete *itor;
	}

	if (callTemplate) delete callTemplate;
}