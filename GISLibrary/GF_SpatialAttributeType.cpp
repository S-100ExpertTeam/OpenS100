#include "stdafx.h"
#include "GF_SpatialAttributeType.h"

namespace GF
{
	SpatialAttributeType::SpatialAttributeType()
	{

	}

	SpatialAttributeType::SpatialAttributeType(const SpatialAttributeType& other) :AttributeType(other)
	{
		scaleMinimum = other.scaleMinimum;
		scaleMaximum = other.scaleMaximum;

		//GM::Object geometry;
		geometryID = other.geometryID;

		for (const auto& iter : other.maskReference)
		{
			MaskReference mr = iter;
			maskReference.push_back(mr);
		}
	}

	SpatialAttributeType::~SpatialAttributeType()
	{

	}

	SpatialAttributeType SpatialAttributeType::operator=(const SpatialAttributeType& other)
	{
		AttributeType::operator=(other);

		scaleMinimum = other.scaleMinimum;
		scaleMaximum = other.scaleMaximum;

		//GM::Object geometry;
		geometryID = other.geometryID;

		for (const auto& iter : other.maskReference)
		{
			MaskReference mr = iter;
			maskReference.push_back(mr);
		}

		return *this;
	}

	void SpatialAttributeType::SetGeometryID(std::string value)
	{
		geometryID = value;
	}

	std::string SpatialAttributeType::GetGeometryID()
	{
		return geometryID;
	}
}