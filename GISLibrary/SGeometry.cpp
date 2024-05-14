#include "stdafx.h"
#include "SGeometry.h"


int SGeometry::sizeOfPoint = 10;
// It also has a CPoint arrangement to minimize memory allocation and recovery.
POINT* SGeometry::viewPoints = new CPoint[SGeometry::sizeOfPoint];

SGeometry::SGeometry()
{

}

SGeometry::SGeometry(const SGeometry& other)
	: Geometry(other)
{
	sizeOfPoint = other.sizeOfPoint;

	if (sizeOfPoint > 0)
	{
		viewPoints = new POINT[sizeOfPoint];
		memset(viewPoints, 0x00, sizeof(POINT) * sizeOfPoint);
		memcpy(viewPoints, other.viewPoints, sizeof(POINT) * sizeOfPoint);
	}

	for (const auto& iter : other.additionalInformation)
	{
		GF::InformationType* it = new GF::InformationType(*iter);
		additionalInformation.push_back(it);
	}
}

SGeometry::~SGeometry()
{

}

SGeometry SGeometry::operator=(const SGeometry& other)
{
	if (viewPoints)
	{
		delete[] viewPoints;
		viewPoints = nullptr;
	}

	for (auto& iter : additionalInformation)
	{
		if (iter)
		{
			delete iter;
			iter = nullptr;
		}
	}
	additionalInformation.clear();

	Geometry::operator=(other);

	sizeOfPoint = other.sizeOfPoint;

	if (sizeOfPoint > 0)
	{
		viewPoints = new POINT[sizeOfPoint];
		memset(viewPoints, 0x00, sizeof(POINT) * sizeOfPoint);
		memcpy(viewPoints, other.viewPoints, sizeof(POINT) * sizeOfPoint);
	}

	for (const auto& iter : other.additionalInformation)
	{
		GF::InformationType* it = new GF::InformationType(*iter);
		additionalInformation.push_back(it);
	}

	return *this;
}

void SGeometry::AddInformationType(GF::InformationType* info)
{
	additionalInformation.push_back(info);
}

int SGeometry::GetInformationTypeCount() const
{
	return (int)additionalInformation.size();
}

GF::InformationType* SGeometry::GetInformationType(int index) const
{
	if (index >= 0 && index < GetInformationTypeCount())
	{
		return additionalInformation.at(index);
	}

	return nullptr;
}