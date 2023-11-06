#pragma once
#include "S100_IC_DisplayPlane.h"
#include "S100_IC_SuppressedFeatureLayer.h"

class S100_IC_PredefinedCombination
{
	//@6
	std::string Identifier;
	std::string Name;
	std::string Description;
	std::string UseConditions;
	int* InteroperabilityLevel;
	// IncludedProduct
	std::vector<S100_IC_DisplayPlane*> DisplayPlaneRef;
	// DerivedFeatures
	std::vector<S100_IC_SuppressedFeatureLayer*> SuppressedFeatureLayers;

};

