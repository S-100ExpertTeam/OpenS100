#pragma once

#include "FloatPoint.h"

namespace S100HDF5
{

	class FixedStationInformation
	{
	public:
		FixedStationInformation();
		virtual ~FixedStationInformation();

		int GetNumberOfTimes();
		void SetNumberOfTimes(int num);

		void SetPositioning(S100HDF5::Point* positioning, int numberOfNodes);
		S100HDF5::Point* GetPositioning();
		int GetNumberOfNode();

	private:
		int _NumberOfTimes = 0;
		int _NumberOfNode = 0;
		S100HDF5::Point* _Positioning = nullptr;
	};

}