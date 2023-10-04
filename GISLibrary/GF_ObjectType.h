#pragma once

#include "GF_NamedType.h"
#include "GF_AttributeType.h"
#include "GF_InformationAssociationType.h"
#include "GF_ThematicAttributeType.h"
#include "GF_SimpleAttributeType.h"
#include "GF_ComplexAttributeType.h"

#include <string>
#include <vector>

namespace GF
{
	class ObjectType : 
		public NamedType
	{
	public:
		ObjectType();
		ObjectType(const ObjectType& other);
		virtual ~ObjectType();

	public:
		std::string id;
		std::vector<InformationAssociationType> informationAssociations;
		std::vector<ThematicAttributeType*> attributes;

	public:
		virtual std::string GetID();

		virtual std::wstring GetIDAsWString();

		virtual int GetIDAsInteger();

		virtual int GetInformationRelationCount();

		virtual std::string GetAssociatedInformationID(int index);

		virtual int GetAttributeCount() const;

		virtual ThematicAttributeType* GetAttribute(int index) const;

		virtual std::string GetAttributeValue(int index);

		virtual int GetParentAttributeIndex(int index);

		virtual void AddInformationAssociation(
			std::string informationAssociation,
			std::string role,
			std::string informationID);

		virtual void AddSimpleAttribute(FCD::S100_CD_AttributeValueType valueType, std::string code, std::string value);
		virtual void AddComplexAttribute(ComplexAttributeType* ca);
		virtual ComplexAttributeType* AddComplexAttribute(std::string code);

		void AddAttribute(ThematicAttributeType* item);
	};
}