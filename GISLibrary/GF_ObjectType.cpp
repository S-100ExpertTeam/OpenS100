#include "stdafx.h"
#include "GF_ObjectType.h"
#include <regex>

namespace GF
{
	ObjectType::ObjectType()
	{

	}

	ObjectType::ObjectType(const ObjectType& other)
		: NamedType(other)
	{
		id = other.id;

		for (const auto& iter : other.informationAssociations)
		{
			InformationAssociationType iat = iter;
			informationAssociations.push_back(iat);
		}

		for (const auto& iter : other.attributes)
		{
			ThematicAttributeType* tat = new ThematicAttributeType(*iter);
			attributes.push_back(tat);
		}
	}

	ObjectType::~ObjectType()
	{
		for (auto i = attributes.begin(); i != attributes.end(); i++)
		{
			delete (*i);
			(*i) = nullptr;
		}

		attributes.clear();
	}

	ObjectType ObjectType::operator=(const ObjectType& other)
	{
		informationAssociations.clear();
		for (auto& iter : attributes)
		{
			if (iter)
			{
				delete iter;
				iter = nullptr;
			}
		}
		attributes.clear();

		NamedType::operator=(other);
		
		id = other.id;

		for (const auto& iter : other.informationAssociations)
		{
			InformationAssociationType iat = iter;
			informationAssociations.push_back(iat);
		}

		for (const auto& iter : other.attributes)
		{
			ThematicAttributeType* tat = new ThematicAttributeType(*iter);
			attributes.push_back(tat);
		}

		return *this;
	}

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
		return (int)informationAssociations.size();
	}

	InformationAssociationType ObjectType::getInformationAssociation(int index)
	{
		if (index < 0 || index >= informationAssociations.size()) {
			return InformationAssociationType();
		}

		return informationAssociations.at(index);
	}

	std::string ObjectType::GetAssociatedInformationID(int index) 
	{
		if (index >= 0 && index < GetInformationRelationCount())
		{
			return informationAssociations.at(index).GetInformationID();
		}

		return "";
	}

	int ObjectType::GetAttributeCount() const
	{
		return (int)attributes.size();
	}

	ThematicAttributeType* ObjectType::GetAttribute(int index) const
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

	GF::SimpleAttributeType* ObjectType::AddSimpleAttribute(FCD::S100_CD_AttributeValueType valueType, std::string code, std::string value)
	{
		auto sa = new SimpleAttributeType(valueType, value);
		sa->SetCode(code);
		
		attributes.push_back(sa);

		return sa;
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

	void ObjectType::AddAttribute(ThematicAttributeType* item)
	{
		attributes.push_back(item);
	}
}