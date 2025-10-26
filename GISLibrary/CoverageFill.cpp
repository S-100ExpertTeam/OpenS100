#include "stdafx.h"
#include "CoverageFill.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

namespace Part9a
{
    // CoverageFill class implementation
    CoverageFill::CoverageFill(const std::string& attributeCode, const std::string& uom, const std::string& placement)
        : attributeCode(attributeCode), uom(uom), placement(placement) {
    }

    void CoverageFill::init()
    {
        Command::init();
        attributeCode.clear();
        uom.clear();
        placement.clear();
    }

    void CoverageFill::execute()
    {
    }

    void CoverageFill::parse(const std::string& input)
    {
        setPresent();
        // CoverageFill:attributeCode[,uom[,placement]] 
        auto tokens = LatLonUtility::Split(input, ",");
        if (tokens.size() > 0)
        {
            attributeCode = tokens[0];
            if (tokens.size() > 1)
            {
                uom = tokens[1];
                if (tokens.size() > 2)
                {
                    placement = tokens[2];
                }
            }
        }
        else
        {
            init(); // Reset to default values
        }
    }
}