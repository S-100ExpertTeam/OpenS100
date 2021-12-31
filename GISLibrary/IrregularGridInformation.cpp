#include "stdafx.h"
#include "IrregularGridInformation.h"

using namespace S100HDF5;

IrregularGridInformation::IrregularGridInformation()
{
}


IrregularGridInformation::~IrregularGridInformation()
{
	delete[] _Positioning;
	_Positioning = nullptr;
}


int IrregularGridInformation::GetNumberOfNodes()
{
	return _NumberOfNodes;
}


S100HDF5::Point* IrregularGridInformation::GetPositioning()
{
	return _Positioning;
}


void IrregularGridInformation::SetPositioning(S100HDF5::Point* positioning, int numberOfNodes)
{
	_Positioning = positioning;
	_NumberOfNodes = numberOfNodes;
}