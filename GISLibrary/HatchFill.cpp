#include "stdafx.h"
#include "HatchFill.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

namespace Part9a
{
    // HatchFill class implementation
    HatchFill::HatchFill(const Part9a::Vector& direction, double distance, const std::string& lineStyle1, const std::string& lineStyle2)
        : direction(direction), distance(distance), lineStyle1(lineStyle1), lineStyle2(lineStyle2) {
    }

    void HatchFill::init()
    {
        Command::init();
        direction.Set(0.0, 0.0);
        distance = 0.0;
        lineStyle1.clear();
        lineStyle2.clear();
    }

    void HatchFill::execute() {
    }

    void HatchFill::parse(const std::string& input)
    {
        setPresent();
        // HatchFill:direction,distance,lineStyle[,lineStyle] 
        auto tokens = LatLonUtility::Split(input, ",");
        if (tokens.size() >= 4)
        {
            try
            {
                direction.Set(std::stod(tokens[0]), std::stod(tokens[1]));
                distance = std::stod(tokens[2]);
                lineStyle1 = tokens[3];

                if (tokens.size() == 5)
                {
                    lineStyle2 = tokens[4];
                }
            }
            catch (...) {
                init();
            }
        }
    }
}
