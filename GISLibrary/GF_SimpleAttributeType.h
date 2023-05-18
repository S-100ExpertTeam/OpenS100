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
		virtual ~SimpleAttributeType();

	public:
		FCD::S100_CD_AttributeValueType valueType = FCD::S100_CD_AttributeValueType::none;
		std::string value;

		virtual std::string GetValue() override { return value; }
		virtual bool IsSimple() override { return true; }
	};
}