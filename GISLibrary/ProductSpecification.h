#pragma once

#include <memory>
#include <string>
#include "CompliancyCategory.h"
#include "S100_Date.h"
#include "pugiSupportClass.h"

namespace S100 {
    class ProductSpecification
    {
    public:
        std::shared_ptr<std::string> Name;
        std::shared_ptr<std::string> Version;
        std::shared_ptr<S100_Date> Date;
        std::string ProductIdentifier;
        int Number = -1;
        std::shared_ptr<CompliancyCategory> compliancyCategory;

        void GetContents(pugi::xml_node& node);
        void Save(pugi::xml_node& node);
    };
}
