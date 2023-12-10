#pragma once

#include <memory>
#include <vector>
#include "TemporalExtent.h"
#include "EX_BoundingPolygon.h"

namespace S100 {
    class DataCoverage
    {
    public:
        EX_BoundingPolygon BoundingPolygon;
        std::shared_ptr<TemporalExtent> temporalExtent;
        std::shared_ptr<int> OptimumDisplayScale;
        std::shared_ptr<int> MaximumDisplayScale;
        std::shared_ptr<int> MinimumDisplayScale;
        std::vector<double> ApproximateGridResolution;

        void GetContents(pugi::xml_node& node);
    };
}
