#pragma once

#include "GF_AttributeType.h"
#include "GF_MaskReference.h"
#include "GM_Object.h"

#include "../FeatureCatalog/SpatialPrimitiveType.h"

#include "../LuaScriptingReference/spatial_association.h"

#include <vector>
#include <optional>
#include <memory>

namespace GF
{ 

class SpatialAttributeType :
    public AttributeType
{
public:
    SpatialAttributeType();
    SpatialAttributeType(const SpatialAttributeType& other);
    virtual ~SpatialAttributeType();

public:
    std::optional<int> scaleMinimum;
    std::optional<int> scaleMaximum;

    // Owned placeholder (carries the geometry id when the dataset has not resolved it).
    std::unique_ptr<GM::Object> geometry;
    // Non-owning reference to a geometry owned by the dataset (S10XGML::geometries).
    GM::Object* sharedGeometry = nullptr;

    std::vector<MaskReference> maskReference;

public:
    virtual SpatialAttributeType operator=(const SpatialAttributeType& other);

public:
    void SetGeometryID(std::string value);
    std::string GetGeometryID();

	void SetGMGeometry(GM::Object* geometry);
	GM::Object* GetGMGeometry();

    std::optional<int> getScaleMinimum();
    std::optional<std::string> getScaleMinimumAsString();
    void setScaleMinimum(int value);

    std::optional<int> getScaleMaximum();
    std::optional<std::string> getScaleMaximumAsString();
    void setScaleMaximum(int value);
    SpatialPrimitiveType getSpatialPrimitiveType();
    spatial_association getLuaSpatialAssociation();
};

}