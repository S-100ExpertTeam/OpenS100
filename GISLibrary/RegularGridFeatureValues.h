#pragma once

#include "FloatValues.h"

#include <vector>

namespace S100HDF5
{

	class RegularGridFeatureValues
	{
	public:
		RegularGridFeatureValues();
		virtual ~RegularGridFeatureValues();

		void AddFloatValues(FloatValues* floatValues);
		FloatValues* GetFloatValues(int index);
		int GetFloatValuesCount();

	private:
		std::vector<FloatValues*> _Values;
	};

}