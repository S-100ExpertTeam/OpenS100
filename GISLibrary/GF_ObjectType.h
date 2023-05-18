#pragma once

#include "GF_NamedType.h"
#include "GF_AttributeType.h"
#include "GF_InformationAssociationType.h"
#include "GF_ThematicAttributeType.h"

#include <string>
#include <vector>

namespace GF
{
	class ObjectType : 
		public NamedType
	{
	public:
		ObjectType();
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

		virtual int GetAttributeCount();

		virtual ThematicAttributeType* GetAttribute(int index);

		virtual std::string GetAttributeValue(int index);

		virtual int GetParentAttributeIndex(int index);

		virtual void AddInformationAssociation(
			std::string informationAssociation,
			std::string role,
			std::string informationID);

		virtual void AddSimpleAttribute()
	};
}