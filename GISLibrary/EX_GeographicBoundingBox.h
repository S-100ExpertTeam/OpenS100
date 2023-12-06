#pragma once

#include <string>
#include "pugixml.hpp"
#include "pugiSupportClass.h"

namespace S100 {
    class EX_GeographicBoundingBox {
    public:
        double WestBoundLongitude;
        double EastBoundLongitude;
        double SouthBoundLatitude;
        double NorthBoundLatitude;

        void GetContents(pugi::xml_node& node);
    };
}
