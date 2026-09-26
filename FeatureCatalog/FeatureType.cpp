#include "stdafx.h"
#include "StringUtil.h"
#include "FeatureType.h"

#include "../LatLonUtility/LatLonUtility.h"

#include <iostream>

FeatureType::FeatureType()
{

}

FeatureType::~FeatureType()
{
	for (auto i = vecFeatureBinding.begin(); i != vecFeatureBinding.end(); i++)
	{
		delete *i;
	}
}

void FeatureType::GetContents(pugi::xml_node& node)
{
	((S100ObjectType*)this)->S100ObjectType::GetContents(node);

	for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
	{
		std::string instructionName = instruction.name();
		instructionName = LatLonUtility::DeleteXMLNamespace(instructionName);
		if (instructionName == "featureUseType")
		{
			featureUseType.GetContents(instruction);
		}
		else if (instructionName == "featureBinding")
		{
			auto featureBinding = new FeatureBinding();
			featureBinding->GetContents(instruction);
			if (auto targetFB = CanMerged(*featureBinding))
			{
				targetFB->AppendFeatureBinding(*featureBinding);
				delete featureBinding;
				featureBinding = nullptr;
			}
			else
			{
				InsertFeatureBinding(featureBinding);
			}
		}
		else if (instructionName == "permittedPrimitives")
		{
			auto strPermittedPrimitive = instruction.child_value();

			auto type = StringToSpatialPrimitiveType(std::string(strPermittedPrimitive));

			permittedPrimitives.push_back(type);
		}
		else if (instructionName == "S100FC:superType")
		{
			superType = instruction.child_value();
		}
		else if (instructionName == "S100FC:subType")
		{
			subType.push_back(instruction.child_value());
		}
	}
}

const std::string& FeatureType::GetSuperType()
{
	return superType;
}

void FeatureType::SetSuperType(std::string value)
{
	superType = value;
}

//FeatureBinding* FeatureType::GetFeatureBinding(std::string featureTypeCode)
//{
//	auto item = featureBinding.find(featureTypeCode);
//	if (item != featureBinding.end())
//	{
//		return item->second;
//	}
//
//	return nullptr;
//}
//
//FeatureBinding* FeatureType::GetFeatureBinding(std::string featureTypeCode)
//{
//	return GetFeatureBinding(featureTypeCode);
//}

std::list<std::string>& FeatureType::GetSubTypePointer()
{
	return subType;
}

FeatureUseType& FeatureType::GetFeatureUseTypePointer()
{
	return featureUseType;
}

std::list<FeatureBinding*>& FeatureType::GetFeatureBindingPointer()
{
	return vecFeatureBinding;
}

std::list<SpatialPrimitiveType>& FeatureType::GetPermittedPrimitivesPointer()
{
	return permittedPrimitives;
}

void FeatureType::InsertFeatureBinding(FeatureBinding* value)
{
	for (auto i = vecFeatureBinding.begin(); i != vecFeatureBinding.end(); i++) {
		if ((*i)->IsSameAssociation(*value)) {
			delete value;
			return;
		}
	}

	vecFeatureBinding.push_back(value);
}

FeatureBinding* FeatureType::CanMerged(FeatureBinding& fb)
{
	for (auto i = vecFeatureBinding.begin(); i != vecFeatureBinding.end(); i++)
	{
		auto currentFB = *i;
		if (currentFB->IsSameAssociation(fb))
		{
			return currentFB;
		}
	}

	return nullptr;
}

std::wstring FeatureType::GetSuperTypeW()
{
	return toWide(GetSuperType());
}

void FeatureType::SetSuperType(const char* value)
{
	std::string s(value ? value : "");
	SetSuperType(s);
}

void FeatureType::SetSuperType(std::wstring value)
{
	std::string s = toUtf8(value);
	SetSuperType(s);
}
