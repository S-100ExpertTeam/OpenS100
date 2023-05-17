#pragma once

#include "GF_ObjectType.h"
#include "GF_AttributeType.h"
#include "GM_Object.h"
#include "GF_PropertyType.h"
#include "GF_SpatialAttributeType.h"

#include "../FeatureCatalog/SpatialPrimitiveType.h"

#include "SGeometry.h"

#include <string>
#include <vector>

namespace GF
{
	class FeatureType : public ObjectType
	{
	public:
		FeatureType();
		~FeatureType();

	public:
		std::vector<PropertyType*> attributes;
		SpatialAttributeType* spatial = nullptr;

	public:
		virtual bool IsNoGeometry() {
			return false;
		}

		virtual int GetFeatureRelationCount() {
			return 0;
		}

		virtual std::string GetAssociatedFeatureID(int index) {
			return "";
		}

		virtual SpatialPrimitiveType GetSpatialPrimitiveType() {
			return SpatialPrimitiveType::none;
		}

		virtual SGeometry* GetGeometry() {
			return nullptr;
		}

		virtual void SetGeometryID(std::string value);

		// Need release by a user.
		virtual GM::Object* GetGMGeometry() { return nullptr; }

		virtual int GetAttributeCount() override;
	};
}