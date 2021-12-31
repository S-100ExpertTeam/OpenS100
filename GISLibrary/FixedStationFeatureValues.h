#pragma once

#include "FloatValues.h"

#include <vector>
#include <string>

namespace S100HDF5
{

	class FixedStationFeatureValues
	{
	public:
		FixedStationFeatureValues();
		virtual ~FixedStationFeatureValues();

		void SetName(std::string subregionName);
		std::string GetName();

		void AddFloatValues(FloatValues* floatValues);
		FloatValues* GetFloatValues(int index);
		int GetFloatValuesCount();

	private:
		std::vector<FloatValues*> _Values;
		std::string _SubregionName;
	};

}