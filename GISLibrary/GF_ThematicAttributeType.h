#pragma once
#include "GF_AttributeType.h"

namespace GF
{

    class ThematicAttributeType :
        public AttributeType
    {
    public:
        ThematicAttributeType();
        ThematicAttributeType(const ThematicAttributeType& other);
        virtual ~ThematicAttributeType();

    public:
        virtual ThematicAttributeType operator=(const ThematicAttributeType& other);
        virtual ThematicAttributeType* clone();
    };

}