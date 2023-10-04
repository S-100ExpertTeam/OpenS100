#include "stdafx.h"
#include "GF_FeatureType.h"

#include <regex>

namespace GF
{
	FeatureType::FeatureType()
	{

	}

	FeatureType::~FeatureType()
	{
		delete spatial;

		if (geometry)
		{
			geometry->Release();
			delete geometry;
			geometry = nullptr;
		}
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
}