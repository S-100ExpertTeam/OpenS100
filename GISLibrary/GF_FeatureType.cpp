#include "stdafx.h"
#include "GF_FeatureType.h"

namespace GF
{
	FeatureType::FeatureType()
	{

	}

	FeatureType::~FeatureType()
	{
		delete spatial;
	}

	//int FeatureType::GetAttributeCount()
	//{
	//	return attributes.size();
	//}

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

	void FeatureType::SetGeometryID(std::string value)
	{
		if (nullptr == spatial)
		{
			spatial = new SpatialAttributeType();
		}

		spatial->SetGeometryID(value);
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
		return featureAssociations.size();
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
		return SpatialPrimitiveType::none;
	}

	SGeometry* FeatureType::GetGeometry()
	{
		return nullptr;
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
}