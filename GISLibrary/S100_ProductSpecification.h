#pragma once

#include <memory>
#include <string>
#include "S100_CompliancyCategory.h"
#include "S100_Date.h"
#include "pugiSupportClass.h"

namespace S100 {
    class S100_ProductSpecification
    {
    public:
        std::shared_ptr<std::string> Name;
        std::shared_ptr<std::string> Version;
        std::shared_ptr<S100_Date> Date;
        std::string ProductIdentifier;
        int Number;
        std::shared_ptr<S100_CompliancyCategory> CompliancyCategory;

        void GetContents(pugi::xml_node& node);
    };
}
