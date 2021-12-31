#include "stdafx.h"
#include "IrregularGridFeature.h"


using namespace S100HDF5;


IrregularGridFeature::IrregularGridFeature()
{
}


IrregularGridFeature::~IrregularGridFeature()
{
	delete _IrregularGridInformation;
	_IrregularGridInformation = nullptr;

	for (auto i = _Values.begin(); i != _Values.end(); i++)
	{
		delete *i;
		*i = nullptr;
	}
}


void IrregularGridFeature::SetIrregularGridInformation(IrregularGridInformation* irregularGridInformation)
{
	if (!irregularGridInformation)
	{
		return;
	}

	_IrregularGridInformation = irregularGridInformation;

	//_IrregularGridInformation = new IrregularGridInformation();
	//_IrregularGridInformation->SetPositioning(irregularGridInformation->GetPositioning(), irregularGridInformation->GetNumberOfNodes());
}


void IrregularGridFeature::AddIrregularGridFeatureValues(IrregularGridFeatureValues* irregularGridFeatureValues)
{
	_Values.push_back(irregularGridFeatureValues);
}

IrregularGridInformation* IrregularGridFeature::GetIrregularGridInformation()
{
	return _IrregularGridInformation;
}


IrregularGridFeatureValues* IrregularGridFeature::GetIrregularGridFeatureValues(int index)
{
	try
	{
		return _Values.at(index);
	}
	catch (std::out_of_range e)
	{
		return nullptr;
	}

	return nullptr;
}


IrregularGridFeatureValues* IrregularGridFeature::GetNearestIrregularGridFeatureValues(time_t time)
{
	int findIndex = 0;
	__int64 shortestDt = 0;

	for (int i = 0; i < _Values.size(); i++)
	{
		time_t currentDt = abs(_Values.at(i)->GetTime() - time);
		if (i == 0)
		{
			shortestDt = currentDt;
		}
		else if (shortestDt > currentDt)
		{
			shortestDt = currentDt;
			findIndex = i;
		}
	}

	return GetIrregularGridFeatureValues(findIndex);
}