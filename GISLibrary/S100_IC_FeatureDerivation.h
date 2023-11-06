#pragma once
#include "S100_IC_Feature.h"

class S100_IC_FeatureDerivation
{
	//@7
	std::string Identifier;
	//PrimaryProduct
	std::string PrimaryFeatureCode;
	//PrimarySelector
	//SecondaryProduct
	std::string SecondaryFeatureCode;
	//SecondarySelector
	//OutputProduct
	std::string OutputFeatureCode;
	S100_IC_Feature FeatureRef;
};

