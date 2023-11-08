#pragma once
#include "S100_IC_FeatureDerivation.h"
#include "S100_IC_HybridFeatureCreationRule.h"

namespace S100 {
	class S100_IC_SuppressedFeatureInstance : public S100_IC_FeatureDerivation
	{
	public:
		//Only SimpleRule
		std::shared_ptr<S100_IC_HybridFeatureCreationRule> CreationRule;
	};

}

