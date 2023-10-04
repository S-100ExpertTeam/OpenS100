#pragma once

#include "GF_ObjectType.h"
#include "GF_AttributeType.h"
#include "GM_Object.h"
#include "GF_PropertyType.h"
#include "GF_SpatialAttributeType.h"
#include "GF_FeatureAssociationType.h"

#include "../FeatureCatalog/SpatialPrimitiveType.h"

#include "SGeometry.h"

#include <string>
#include <vector>

namespace GF
{
	class FeatureType : 
		public ObjectType
	{
	public:
		FeatureType();
		virtual ~FeatureType();

	public:
		//std::vector<ThematicAttributeType*> attributes;
		std::vector<FeatureAssociationType> featureAssociations;
		SpatialAttributeType* spatial = nullptr;

		SGeometry* geometry = nullptr;

	public:
		virtual bool IsNoGeometry();

		virtual int GetFeatureRelationCount();

		virtual std::string GetAssociatedFeatureID(int index);

		virtual SpatialPrimitiveType GetSpatialPrimitiveType();

		virtual SGeometry* GetGeometry();
		virtual void SetGeometry(SGeometry* value);

		virtual std::string GetGeometryID();

		virtual int GetGeometryIDAsInt();

		virtual void SetGeometryID(std::string value);

		// Need release by a user.
		virtual GM::Object* GetGMGeometry();

		//virtual int GetAttributeCount();

		//virtual ThematicAttributeType* GetAttribute(int index);

		//virtual std::string GetAttributeValue(int index);

		//virtual int GetParentAttributeIndex(int index);

		virtual void AddFeatureAssociation(
			std::string featureAssociation, 
			std::string role, 
			std::string featureID);
	};
}