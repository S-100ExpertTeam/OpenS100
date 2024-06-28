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

    std::unique_ptr<GM::Object> geometry;
    //std::string geometryID;

    std::vector<MaskReference> maskReference;

public:
    virtual SpatialAttributeType operator=(const SpatialAttributeType& other);

public:
    void SetGeometryID(std::string value);
    std::string GetGeometryID();

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