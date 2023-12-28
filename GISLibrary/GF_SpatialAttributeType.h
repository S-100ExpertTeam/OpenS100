#pragma once

#include "GF_AttributeType.h"
#include "GF_MaskReference.h"
#include "GM_Object.h"

#include <vector>

namespace GF
{ 

class SpatialAttributeType :
    public AttributeType
{
public:
    SpatialAttributeType();
    virtual ~SpatialAttributeType();

public:
    // if value = -1, it doesn't exist
    int scaleMinimum = -1;
    int scaleMaximum = -1;

    //GM::Object geometry;
    std::string geometryID;

    std::vector<MaskReference> maskReference;

public:
    virtual SpatialAttributeType* Clone() const;

public:
    void SetGeometryID(std::string value);
    std::string GetGeometryID();
};

}