#pragma once

#include <vector>

#include "S100HDF5Feature.h"
#include "RegularGridInformation.h"
#include "RegularGridFeatureValues.h"
#include "TimeInformation.h"

namespace S100HDF5
{

	class RegularGridFeature :
		public S100HDF5Feature
	{
	public:
		RegularGridFeature();
		virtual ~RegularGridFeature();

		void SetRegularGridInformation(RegularGridInformation* regularGridInformation);
		RegularGridInformation* GetRegularGridInformation();

		void AddRegularGridFeatureValues(RegularGridFeatureValues* featureValues);
		RegularGridFeatureValues* GetRegularGridFeatureValues(int index);
		RegularGridFeatureValues* GetNearestRegularGridFeatureValues(time_t time, TimeInformation* timeInformation);

	private:
		RegularGridInformation* _RegularGridInformation = nullptr;
		std::vector<RegularGridFeatureValues* > _Values;
	};

}