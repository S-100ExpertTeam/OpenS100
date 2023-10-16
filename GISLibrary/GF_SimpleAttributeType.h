#pragma once

#include "GF_ThematicAttributeType.h"

#include "../FeatureCatalog/S100_CD_AttributeValueType.h"

#include <string>

namespace GF
{
	class SimpleAttributeType : 
		public ThematicAttributeType
	{
	public:
		SimpleAttributeType();
		SimpleAttributeType(FCD::S100_CD_AttributeValueType valueType, std::string value);
		virtual ~SimpleAttributeType();

	public:
		FCD::S100_CD_AttributeValueType valueType = FCD::S100_CD_AttributeValueType::none;
		std::string value;

		virtual std::string GetValue() override;
		virtual bool IsSimple() override;
		virtual ThematicAttributeType* clone() override;
	};
}