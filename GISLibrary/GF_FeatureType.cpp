#include "stdafx.h"
#include "GF_FeatureType.h"

#include "SPoint.h"
#include "SCompositeCurve.h"
#include "SSurface.h"
#include "SMultiPoint.h"
#include "SCurve.h"
#include "SCoverage.h"

#include <regex>

namespace GF
{
	FeatureType::FeatureType()
	{

	}

	FeatureType::FeatureType(const FeatureType& other)
		: ObjectType(other)
	{		
		for (const auto& iter : other.featureAssociations)
		{
			FeatureAssociationType fat = iter;
			featureAssociations.push_back(fat);
		}
		
		if (other.spatial)
			spatial = new SpatialAttributeType(*other.spatial);
		
		if (other.geometry)
		{
			switch (other.geometry->GetType())
			{				
			case SGeometryType::Point:
			{
				SPoint* pt = (SPoint*)other.geometry;
				geometry = (pt) ? new SPoint(*pt) : nullptr;
			}
			break;
			case SGeometryType::CompositeCurve:
			{
				SCompositeCurve* cc = (SCompositeCurve*)other.geometry;
				geometry = (cc) ? new SCompositeCurve(*cc) : nullptr;
			}
			break;
			case SGeometryType::Surface:
			{
				SSurface* sf = (SSurface*)other.geometry;
				geometry = (sf) ? new SSurface(*sf) : nullptr;
			}
			break;
			case SGeometryType::MultiPoint:
			{
				SMultiPoint* mp = (SMultiPoint*)other.geometry;
				geometry = (mp) ? new SMultiPoint(*mp) : nullptr;
			}
			break;
			case SGeometryType::Curve:
			{
				SCurve* cv = (SCurve*)other.geometry;
				geometry = (cv) ? new SCurve(*cv) : nullptr;
			}
			break;
			case SGeometryType::Coverage:
			{
				SCoverage* cr = (SCoverage*)other.geometry;
				geometry = (cr) ? new SCoverage(*cr) : nullptr;
			}
			break;
			default:
				break;
			}
		}
	}

	FeatureType::~FeatureType()
	{
		featureAssociations.clear();
		delete spatial;

		if (geometry)
		{
			geometry->Release();
			delete geometry;
			geometry = nullptr;
		}
	}

	int FeatureType::GetAttributeCount()
	{
		return ObjectType::GetAttributeCount();
	}

	//ThematicAttributeType* FeatureType::GetAttribute(int index)
	//{
	//	if (index >= 0 && index < GetAttributeCount())
	//	{
	//		return attributes.at(index);
	//	}

	//	return nullptr;
	//}

	//std::string FeatureType::GetAttributeValue(int index)
	//{
	//	auto attr = GetAttribute(index);
	//	if (attr)
	//	{
	//		if (attr->IsSimple())
	//		{
	//			return attr->GetValue();
	//		}
	//	}

	//	return "";
	//}

	//int FeatureType::GetParentAttributeIndex(int index)
	//{
	//	return 0;
	//}

	std::string FeatureType::GetGeometryID()
	{
		if (spatial)
		{
			return spatial->GetGeometryID();
		}

		return "";
	}

	int FeatureType::GetGeometryIDAsInt()
	{
		std::string digitID = std::regex_replace(GetGeometryID(), std::regex(R"([\D])"), "");
		return std::stoi(digitID);
	}

	void FeatureType::SetGeometryID(std::string value)
	{
		if (nullptr == spatial)
		{
			spatial = new SpatialAttributeType();
		}

		spatial->SetGeometryID(value);
	}

	FeatureType FeatureType::operator=(const FeatureType& other)
	{
		featureAssociations.clear();
		if (spatial)
		{
			delete spatial;
			spatial = nullptr;
		}

		if (geometry)
		{
			delete geometry;
			geometry = nullptr;
		}

		ObjectType::operator=(other);

		for (const auto& iter : other.featureAssociations)
		{
			FeatureAssociationType fat = iter;
			featureAssociations.push_back(fat);
		}

		if (other.spatial)
			spatial = new SpatialAttributeType(*other.spatial);

		if (other.geometry)
		{
			switch (other.geometry->GetType())
			{
			case SGeometryType::Point:
			{
				SPoint* pt = (SPoint*)other.geometry;
				geometry = (pt) ? new SPoint(*pt) : nullptr;
			}
			break;
			case SGeometryType::CompositeCurve:
			{
				SCompositeCurve* cc = (SCompositeCurve*)other.geometry;
				geometry = (cc) ? new SCompositeCurve(*cc) : nullptr;
			}
			break;
			case SGeometryType::Surface:
			{
				SSurface* sf = (SSurface*)other.geometry;
				geometry = (sf) ? new SSurface(*sf) : nullptr;
			}
			break;
			case SGeometryType::MultiPoint:
			{
				SMultiPoint* mp = (SMultiPoint*)other.geometry;
				geometry = (mp) ? new SMultiPoint(*mp) : nullptr;
			}
			break;
			case SGeometryType::Curve:
			{
				SCurve* cv = (SCurve*)other.geometry;
				geometry = (cv) ? new SCurve(*cv) : nullptr;
			}
			break;
			case SGeometryType::Coverage:
			{
				SCoverage* cr = (SCoverage*)other.geometry;
				geometry = (cr) ? new SCoverage(*cr) : nullptr;
			}
			break;
			default:
				break;
			}
		}

		return *this;
	}

	bool FeatureType::IsNoGeometry()
	{
		if (false == spatial)
		{
			return true;
		}

		return false;
	}

	int FeatureType::GetFeatureRelationCount()
	{
		return (int)featureAssociations.size();
	}

	FeatureAssociationType FeatureType::getFeatureAssociation(int index)
	{
		if (index < 0 || index >= (int)featureAssociations.size())
		{
			return FeatureAssociationType();
		}

		return featureAssociations.at(index);
	}

	std::string FeatureType::GetAssociatedFeatureID(int index)
	{
		if (index >= 0 && index < GetFeatureRelationCount())
		{
			return featureAssociations.at(index).GetFeatureID();
		}

		return "";
	}

	SpatialPrimitiveType FeatureType::GetSpatialPrimitiveType()
	{
		if (spatial)
		{
			return spatial->getSpatialPrimitiveType();
		}

		return SpatialPrimitiveType::none;
	}

	SGeometry* FeatureType::GetGeometry()
	{
		return geometry;
	}

	void FeatureType::SetGeometry(SGeometry* value)
	{
		geometry = value;
	}

	GM::Object* FeatureType::GetGMGeometry()
	{
		return nullptr;
	}

	void FeatureType::AddFeatureAssociation(
		std::string featureAssociation,
		std::string role,
		std::string featureID)
	{
		FeatureAssociationType fa;
		fa.SetCode(featureAssociation);
		fa.SetRole(role);
		fa.SetFeatureID(featureID);
		featureAssociations.push_back(fa);
	}

	spatial_association FeatureType::getLuaSpatialAssociation()
	{
		if (spatial)
		{
			return spatial->getLuaSpatialAssociation();
		}

		spatial_association sa;
		return sa;
	}
}