#pragma once

#include "IF_ObjectType.h"
#include "IF_Attribute.h"
#include "GM_Geometry.h"

#include "../FeatureCatalog/SpatialPrimitiveType.h"

#include "SGeometry.h"

#include <string>

namespace S100Interface
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
		virtual S100Geometry::Geometry* GetGMGeometry() { return nullptr; }
	};
}