#include "stdafx.h"
#include "S100HDF5File.h"

using namespace S100HDF5;

char* S100HDF5File::XMinAttributeName = "westBoundLongitude";
char* S100HDF5File::YMinAttributeName = "southBoundLatitude";
char* S100HDF5File::XMaxAttributeName = "eastBoundLongitude";
char* S100HDF5File::YMaxAttributeName = "northBoundLatitude";

S100HDF5File::S100HDF5File()
{
}


S100HDF5File::~S100HDF5File()
{
}


MBR S100HDF5File::GetMBR()
{
	return _BoundingBox;
}


void S100HDF5File::SetMBR(MBR* mbr)
{
	_BoundingBox = *mbr;
}