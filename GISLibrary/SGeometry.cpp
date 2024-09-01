#include "stdafx.h"
#include "SGeometry.h"


int SGeometry::sizeOfPoint = 10;
//// It also has a CPoint arrangement to minimize memory allocation and recovery.
POINT* SGeometry::viewPoints = nullptr;// new CPoint[SGeometry::sizeOfPoint];

SGeometry::SGeometry()
{

}

SGeometry::SGeometry(const SGeometry& other)
	: Geometry(other)
{
	int cnt = other.GetInformationTypeCount();
	for (int i = 0; i < cnt; i++) {
		auto item = new GF::InformationType(*other.GetInformationType(i));
		AddInformationType(item);
	}
}

SGeometry::~SGeometry()
{
	additionalInformation.clear();
}

SGeometry SGeometry::operator=(const SGeometry& other)
{
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