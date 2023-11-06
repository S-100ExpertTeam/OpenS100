#pragma once
#include "S100_IC_Feature.h"
#include "S100_IC_DrawingInstruction.h"

class S100_IC_SuppressedFeatureLayer
{
	//@5
	std::string Identifier;
	std::string FeatureCode;
	//Product
	std::vector<S100_IC_Feature*> FeatureRef;
	std::vector<S100_IC_DrawingInstruction*> DrawingInstructionRef;
};

