#include "stdafx.h"
#include "GM_MultiPoint.h"

namespace GM
{
	MultiPoint::MultiPoint()
	{

	}

	MultiPoint::~MultiPoint()
	{

	}

	void MultiPoint::Add(double x, double y, double z)
	{
		position.push_back(DirectPosition(x, y, z));
	}

	GeometryType MultiPoint::GetType()
	{
		return GeometryType::MultiPoint;
	}

	std::string MultiPoint::GeometricPrimitiveAsString()
	{
		return "MultiPoint";
	}

	SGeometry* MultiPoint::createGeometry()
	{
		SMultiPoint* result = new SMultiPoint();
		
		for (auto i = position.begin(); i != position.end(); i++) {
			auto x = i->GetX();
			auto y = i->GetY();
			auto z = i->GetZ();
			projection(x, y);
			result->Add(x, y, z);
		}

		result->SetMBR();

		return result;
	}
}