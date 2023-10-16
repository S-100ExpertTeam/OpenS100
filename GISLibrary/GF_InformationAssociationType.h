#pragma once

#include "GF_AssociationType.h"

namespace GF
{

    class InformationAssociationType :
        public AssociationType
    {
    public:
        InformationAssociationType();
        virtual ~InformationAssociationType();

    public:
        std::string informationID = "";

    public:
        std::string GetInformationID();
        void SetInformationID(std::string value);
    };

}