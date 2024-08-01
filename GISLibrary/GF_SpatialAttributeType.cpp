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

		geometry = std::make_unique<GM::Object>();
		*geometry = *other.geometry;

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

		geometry = std::make_unique<GM::Object>();
		*geometry = *other.geometry;

		for (const auto& iter : other.maskReference)
		{
			MaskReference mr = iter;
			maskReference.push_back(mr);
		}

		return *this;
	}

	void SpatialAttributeType::SetGeometryID(std::string value)
	{
		geometry->SetID(value);
		//geometryID = value;
	}

	std::string SpatialAttributeType::GetGeometryID()
	{
		//return geometryID;
		//return geometry.GetID();
		return geometry->GetID();
	}

	std::optional<int> SpatialAttributeType::getScaleMinimum()
	{
		if (scaleMinimum.has_value())
		{
			return scaleMinimum.value();
		}
		
		return std::nullopt;
	}
	
	std::optional<std::string> SpatialAttributeType::getScaleMinimumAsString()
	{
		if (scaleMinimum.has_value())
		{
			return std::to_string(scaleMinimum.value());
		}

		return std::nullopt;
	}

	void SpatialAttributeType::setScaleMinimum(int value)
	{
		scaleMinimum = value;
	}

	std::optional<int> SpatialAttributeType::getScaleMaximum()
	{
		if (scaleMaximum.has_value())
		{
			return scaleMaximum.value();
		}

		return std::nullopt;
	}

	std::optional<std::string> SpatialAttributeType::getScaleMaximumAsString()
	{
		if (scaleMaximum.has_value())
		{
			return std::to_string(scaleMaximum.value());
		}

		return std::nullopt;
	}

	void SpatialAttributeType::setScaleMaximum(int value)
	{
		scaleMaximum = value;
	}

	spatial_association SpatialAttributeType::getLuaSpatialAssociation()
	{
		spatial_association sa;

		switch (getSpatialPrimitiveType())
		{
		case SpatialPrimitiveType::point:
			sa.spatial_id = "Point|" + GetGeometryID();
			sa.spatial_type = "Point";
			break;
		case SpatialPrimitiveType::pointSet:
			sa.spatial_id = "MultiPoint|" + GetGeometryID();
			sa.spatial_type = "MultiPoint";
			break;
		case SpatialPrimitiveType::curve:
			sa.spatial_id = "Curve|" + GetGeometryID();
			sa.spatial_type = "Curve";
			break;
		case SpatialPrimitiveType::surface:
			sa.spatial_id = "Surface|" + GetGeometryID();
			sa.spatial_type = "Surface";
			break;
		case SpatialPrimitiveType::coverage:
			sa.spatial_id = "Coverage|" + GetGeometryID();
			sa.spatial_type = "Coverage";
			break;
		default:
			sa.spatial_id = "None";
			sa.spatial_type = "None";
			break;
		}

		if (geometry)
		{
			if (geometry->isForward())
			{
				sa.orientation = "Forward";
			}
			else
			{
				sa.orientation = "Reverse";
			}
		}

		sa.scale_maximum = getScaleMaximum();
		sa.scale_minimum = getScaleMinimum();

		return sa;
	}

	SpatialPrimitiveType SpatialAttributeType::getSpatialPrimitiveType()
	{
		if (geometry)
		{
			if (typeid(GM::Point) == typeid(*geometry))
			{
				return SpatialPrimitiveType::point;
			}
			else if (typeid(GM::MultiPoint) == typeid(*geometry))
			{
				return SpatialPrimitiveType::pointSet;
			}
			else if (typeid(GM::Curve) == typeid(*geometry) || 
				typeid(GM::CompositeCurve) == typeid(*geometry)) 
				// S100_ArcByCenterPoint
				// S100_CircleByCenterPoint
				// S100_GM_SplineCurve
				// S100_GM_PolynomialSpline
			{
				return SpatialPrimitiveType::curve;
			}
			else if (typeid(GM::Surface) == typeid(*geometry) ||
				typeid(GM::Polygon) == typeid(*geometry))
			{
				return SpatialPrimitiveType::surface;
			}
		}

		return SpatialPrimitiveType::none;
	}
}