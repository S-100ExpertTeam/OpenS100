#pragma once

#include "GF_AssociationType.h"

namespace GF
{

	class FeatureAssociationType : 
		public AssociationType
	{
	public:
		FeatureAssociationType();
		virtual ~FeatureAssociationType();

	public:
		std::string featureID;

	public:
		std::string GetFeatureID();
		void SetFeatureID(std::string value);
	};

}