#pragma once

#include "GF_AssociationType.h"

namespace GF
{

	class FeatureAssociationType : 
		public AssociationType
	{
	public:
		FeatureAssociationType();
		FeatureAssociationType(const FeatureAssociationType& other);
		virtual ~FeatureAssociationType();

	public:
		std::string featureID;

	public:
		virtual FeatureAssociationType operator=(const FeatureAssociationType& other);

	public:
		std::string GetFeatureID();
		void SetFeatureID(std::string value);
	};

}