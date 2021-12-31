#include "stdafx.h"
#include "SENC_NullInstruction.h"
#include "PCOutputSchemaManager.h"

SENC_NullInstruction::SENC_NullInstruction()
{
	type = 0;
}

SENC_NullInstruction::~SENC_NullInstruction()
{

}

void SENC_NullInstruction::DrawInstruction(HDC &hDC, Scaler *scaler, PortrayalCatalogue* pc)
{

}

void SENC_NullInstruction::FromS100Instruction(
	S100_Instruction* s100Instruction, 
	PortrayalCatalogue *pc, 
	PCOutputSchemaManager* output)
{
	output->GetSENCFromS100Common(s100Instruction, this);
}