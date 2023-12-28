#pragma once
#include "GF_PropertyType.h"

namespace GF
{

    class AssociationRole :
        public PropertyType
    {
    public:
        AssociationRole();
        AssociationRole(const AssociationRole& other) : PropertyType(other) {};
        virtual ~AssociationRole();
    };
}