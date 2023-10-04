#pragma once
#include "GF_AttributeType.h"

namespace GF
{

    class ThematicAttributeType :
        public AttributeType
    {
    public:
        ThematicAttributeType();
        virtual ~ThematicAttributeType();

        virtual ThematicAttributeType* clone();
    };

}