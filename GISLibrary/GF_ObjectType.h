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
		virtual ObjectType operator=(const ObjectType& other);

	public:
		virtual std::string GetID();

		virtual std::wstring GetIDAsWString();

		virtual int GetIDAsInteger();

		virtual int GetInformationRelationCount();

		virtual InformationAssociationType getInformationAssociation(int index);

		virtual std::string GetAssociatedInformationID(int index);

		virtual int GetAttributeCount() const;

		virtual ThematicAttributeType* GetAttribute(int index) const;

		virtual std::string GetAttributeValue(int index);

		virtual int GetParentAttributeIndex(int index);

		virtual void AddInformationAssociation(
			std::string informationAssociation,
			std::string role,
			std::string informationID);

		virtual GF::SimpleAttributeType* AddSimpleAttribute(FCD::S100_CD_AttributeValueType valueType, std::string code, std::string value);
		virtual void AddComplexAttribute(ComplexAttributeType* ca);
		virtual ComplexAttributeType* AddComplexAttribute(std::string code);

		virtual void AddAttribute(ThematicAttributeType* item);

		// Return the attribute(Simple or Complex) with the given code and index
		virtual std::optional<ThematicAttributeType*> getThematicAttribute(std::string code, int index);
		virtual std::optional<GF::ComplexAttributeType*> getComplexAttribute(std::string code, int index);
		virtual std::vector<std::string> getSimpleAttributeValues(std::string path, std::string code);
		virtual std::vector<std::string> getSimpleAttributeValues(std::string code);
		virtual int getComplexAttributeCount(std::string path, std::string code);
		virtual int getAttributeCount(std::string code);
	};
}