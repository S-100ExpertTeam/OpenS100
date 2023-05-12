#pragma once

#include "GF_ObjectType.h"
#include "GF_AttributeType.h"
#include "GM_Object.h"

#include "../FeatureCatalog/SpatialPrimitiveType.h"

#include "SGeometry.h"

#include <string>

namespace GF
{
	class FeatureType : public ObjectType
	{
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

		// Need release by a user.
		virtual GM::Object* GetGMGeometry() { return nullptr; }
	};
}