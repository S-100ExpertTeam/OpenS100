#include "stdafx.h"
#include "SGeometry.h"


int SGeometry::sizeOfPoint = 10;
// It also has a CPoint arrangement to minimize memory allocation and recovery.
POINT   *SGeometry::viewPoints = new POINT[10];

SGeometry::SGeometry() 
{

}

SGeometry::~SGeometry()
{

}

void SGeometry::AddInformationType(GF::InformationType* info)
{
	additionalInformation.push_back(info);
}

int SGeometry::GetInformationTypeCount()
{
	return additionalInformation.size();
}

GF::InformationType* SGeometry::GetInformationType(int index)
{
	if (index >= 0 && index < GetInformationTypeCount())
	{
		return additionalInformation.at(index);
	}

	return nullptr;
}