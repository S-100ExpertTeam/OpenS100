#pragma once

#include "GF_AssociationType.h"

namespace GF
{

    class InformationAssociationType :
        public AssociationType
    {
    public:
        InformationAssociationType();
        InformationAssociationType(const InformationAssociationType& other);
        virtual ~InformationAssociationType();

    public:
        std::string informationID = "";

    public:
        virtual InformationAssociationType operator=(const InformationAssociationType& other);

    public:
        std::string GetInformationID();
        void SetInformationID(std::string value);
    };

}