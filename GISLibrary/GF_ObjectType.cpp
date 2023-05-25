#include "stdafx.h"
#include "GF_ObjectType.h"
#include <regex>

namespace GF
{
	ObjectType::ObjectType()
	{

	}

	ObjectType::~ObjectType()
	{
		for (auto i = attributes.begin(); i != attributes.end(); i++)
		{
			delete (*i);
		}
	}

	//AttributeType* ObjectType::GetAttribute(int index)
	//{
	//	//if (index >= 0 && GetAttributeCount())
	//	//{
	//	//	return attributes.at(index);
	//	//}

	//	return nullptr;
	//}

	std::string ObjectType::GetID() 
	{
		return id;
	}

	std::wstring ObjectType::GetIDAsWString() 
	{
		return pugi::as_wide(id);
	}

	int ObjectType::GetIDAsInteger() 
	{
		std::string digitID = std::regex_replace(id, std::regex(R"([\D])"), "");
		return std::stoi(digitID);
	}

	int ObjectType::GetInformationRelationCount() 
	{
		return informationAssociations.size();
	}

	std::string ObjectType::GetAssociatedInformationID(int index) 
	{
		if (index >= 0 && index < GetInformationRelationCount())
		{
			return informationAssociations.at(index).GetInformationID();
		}

		return "";
	}

	int ObjectType::GetAttributeCount() 
	{
		return attributes.size();
	}

	ThematicAttributeType* ObjectType::GetAttribute(int index)
	{
		if (index >= 0 && index < GetAttributeCount())
		{
			return attributes.at(index);
		}

		return nullptr;
	}

	std::string ObjectType::GetAttributeValue(int index) 
	{
		auto attr = GetAttribute(index);
		if (attr)
		{
			if (attr->IsSimple())
			{
				return attr->GetValue();
			}
		}

		return "";
	}

	int ObjectType::GetParentAttributeIndex(int index) 
	{
		return 0;
	}

	void ObjectType::AddInformationAssociation(
		std::string informationAssociation,
		std::string role,
		std::string informationID)
	{
		InformationAssociationType ia;
		ia.SetCode(informationAssociation);
		ia.SetRole(role);
		ia.SetInformationID(informationID);
		informationAssociations.push_back(ia);
	}

	void ObjectType::AddSimpleAttribute(FCD::S100_CD_AttributeValueType valueType, std::string value)
	{
		auto sa = new SimpleAttributeType(valueType, value);
		
		attributes.push_back(sa);
	}

	void ObjectType::AddComplexAttribute(ComplexAttributeType* ca)
	{
		attributes.push_back(ca);
	}

	ComplexAttributeType* ObjectType::AddComplexAttribute(std::string code)
	{
		auto ca = new ComplexAttributeType();
		ca->SetCode(code);
		attributes.push_back(ca);
		return ca;
	}
}