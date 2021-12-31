#pragma once

#include "S100HDF5Feature.h"
#include "IrregularGridInformation.h"
#include "IrregularGridFeatureValues.h"

namespace S100HDF5
{

	class IrregularGridFeature :
		public S100HDF5Feature
	{
	public:
		IrregularGridFeature();
		virtual ~IrregularGridFeature();

		void SetIrregularGridInformation(IrregularGridInformation* irregularGridInformation);
		void AddIrregularGridFeatureValues(IrregularGridFeatureValues* irregularGridFeatureValues);

		IrregularGridInformation* GetIrregularGridInformation();
		IrregularGridFeatureValues* GetIrregularGridFeatureValues(int index);
		IrregularGridFeatureValues* GetNearestIrregularGridFeatureValues(time_t time);

	private:
		IrregularGridInformation* _IrregularGridInformation = nullptr;
		std::vector<IrregularGridFeatureValues* > _Values;
	};

}