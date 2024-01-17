#pragma once
#include "IC_FeatureDerivation.h"
#include "IC_HybridFeatureCreationRule.h"

namespace S100 {
	class IC_SuppressedFeatureInstance : public IC_FeatureDerivation
	{
	public:
		//Only SimpleRule
		std::shared_ptr<IC_HybridFeatureCreationRule> CreationRule;
	};

}

