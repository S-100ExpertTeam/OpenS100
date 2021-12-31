#include "stdafx.h"
#include "RegularGridInformation.h"


using namespace S100HDF5;


RegularGridInformation::RegularGridInformation()
{
}


RegularGridInformation::~RegularGridInformation()
{
}


void RegularGridInformation::SetNumPointLongitudinal(int num)
{
	_NumPointLongitudinal = num;
}


int RegularGridInformation::GetNumPointLongitudinal()
{
	return _NumPointLongitudinal;
}


void RegularGridInformation::SetNumPointLatitudinal(int num)
{
	_NumPointLatitudinal = num;
}


int RegularGridInformation::GetNumPointLatitudinal()
{
	return _NumPointLatitudinal;
}


void RegularGridInformation::SetGridSpacingLongitudinal(float spacing)
{
	_GridSpacingLongitudinal = spacing;
}


float RegularGridInformation::GetGridSpacingLongitudinal()
{
	return _GridSpacingLongitudinal;
}


void RegularGridInformation::SetGridSpacingLatitudinal(float spacing)
{
	_GridSpacingLatitudinal = spacing;
}


float RegularGridInformation::GetGridSpacingLatitudinal()
{
	return _GridSpacingLatitudinal;
}


void RegularGridInformation::SetGridOriginLongitude(float origin)
{
	_GridOriginLongitude = origin;
}


float RegularGridInformation::GetGridOgirinLongitude()
{
	return _GridOriginLongitude;
}


void RegularGridInformation::SetGridOriginLatitude(float origin)
{
	_GridOriginLatitude = origin;
}


float RegularGridInformation::GetGridOriginLatitude()
{
	return _GridOriginLatitude;
}