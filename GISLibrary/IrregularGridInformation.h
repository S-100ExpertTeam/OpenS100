#pragma once

#include "FloatPoint.h"

namespace S100HDF5
{

	class IrregularGridInformation
	{
	public:
		IrregularGridInformation();
		virtual ~IrregularGridInformation();

		int GetNumberOfNodes();
		S100HDF5::Point* GetPositioning();

		void SetPositioning(S100HDF5::Point* positioning, int numberOfNodes);

	private:
		int _NumberOfNodes = 0;
		S100HDF5::Point* _Positioning = nullptr;
	};

}