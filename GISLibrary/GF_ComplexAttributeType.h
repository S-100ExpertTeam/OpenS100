#pragma once

#include "GF_ThematicAttributeType.h"

#include <string>
#include <vector>
#include <optional>

namespace GF
{
	class SimpleAttribute;

	class ComplexAttributeType : public ThematicAttributeType
	{
	public:
		ComplexAttributeType();
		ComplexAttributeType(const ComplexAttributeType& other);
		virtual ~ComplexAttributeType();

	public:
		std::vector<ThematicAttributeType*> carries;

	public:
		std::optional<ThematicAttributeType*> getThematicAttribute(std::string code, int index);
		std::optional<ComplexAttributeType*> getComplexAttribute(std::string code, int index);
		std::vector<std::string> getAttributeValues(std::string code);

	public:
		virtual int GetSubAttributeCount() const;
		virtual int GetSubAttributeCount(std::string code) const;
		virtual ThematicAttributeType* GetSubAttribute(int index) const;
		virtual bool IsSimple();
		virtual ThematicAttributeType* AddSubAttribute(ThematicAttributeType* subAttribute);
		virtual SimpleAttribute* AddSubSimpleAttribute(FCD::S100_CD_AttributeValueType valueType, std::string code, std::string value);
		virtual ComplexAttributeType* AddComplexAttribute(std::string code);
		virtual ThematicAttributeType* clone() override;
	};
}