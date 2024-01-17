#pragma once
#include "IC_HybridFeatureCreationRule.h"
#include "IC_FeatureDerivation.h"

namespace  S100{

	class IC_HybridFeature :public IC_FeatureDerivation
	{
	public:
		// CompleteRule OR ThematicRule
		std::shared_ptr<IC_HybridFeatureCreationRule> creationRule;
	};
}

