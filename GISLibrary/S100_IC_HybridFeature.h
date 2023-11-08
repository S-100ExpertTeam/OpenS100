#pragma once
#include "S100_IC_HybridFeatureCreationRule.h"
#include "S100_IC_FeatureDerivation.h"

namespace  S100{

	class S100_IC_HybridFeature :public S100_IC_FeatureDerivation
	{
	public:
		// CompleteRule OR ThematicRule
		std::shared_ptr<S100_IC_HybridFeatureCreationRule> creationRule;
	};
}

