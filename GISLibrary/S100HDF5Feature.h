#pragma once

#include <string>

#include "TimeInformation.h"
#include "DataCodingFormat.h"

namespace S100HDF5
{

	class S100HDF5Feature
	{
	public:
		S100HDF5Feature();
		virtual ~S100HDF5Feature();

		void SetDataCodingFormat(DataCodingFormat dataCodingFormat);
		void SetName(std::string name);
		void SetTimeInformation(TimeInformation* timeInformation);

		DataCodingFormat GetDataCodingFormat();
		std::string GetName();
		TimeInformation* GetTimeInformation();

	private:
		DataCodingFormat _DataCodingFormat = DataCodingFormat::None;
		std::string _Name = "";
		TimeInformation* _TimeInformation = nullptr;
	};

}