#pragma once

#include <string>
#include <memory>
#include <pugixml.hpp>

namespace S100
{
    class IC_HybridFeatureCreationRule
    {
    public:
        std::shared_ptr<int> InteroperabilityLevel;
        std::string RuleIdentifier;

        void GetContents(pugi::xml_node& node);
    };
}
