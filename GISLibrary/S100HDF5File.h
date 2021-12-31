#pragma once

#include "..\\GeoMetryLibrary\\MBR.h"

#include "PickReportResult.h"

namespace S100HDF5
{

	class S100HDF5File
	{
	public:
		S100HDF5File();
		virtual ~S100HDF5File();
		
		virtual bool Open(std::string path) { return true; }
		virtual PickReportResult PickReport(float x, float y, time_t time, const char* featureName) { return PickReportResult(); }

		MBR GetMBR();
		void SetMBR(MBR* mbr);

		static char* XMinAttributeName;
		static char* XMaxAttributeName;
		static char* YMinAttributeName;
		static char* YMaxAttributeName;

	private: 
		MBR _BoundingBox;
	};

}