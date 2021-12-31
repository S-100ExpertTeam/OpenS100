#pragma once

#include <ctime>
#include <vector>

#include "FloatValues.h"

namespace S100HDF5
{

	class IrregularGridFeatureValues
	{
	public:
		IrregularGridFeatureValues();
		virtual ~IrregularGridFeatureValues();

		void SetTime(time_t TimePoint);
		void AddFloatValues(FloatValues* Values);

		time_t GetTime();
		FloatValues* GetFloatValues(int index);
		int GetFloatValuesCount();

		static char* TimePointAttributeName;

	private:
		time_t _TimePoint = 0;
		std::vector<FloatValues*> _Values;
	};

}