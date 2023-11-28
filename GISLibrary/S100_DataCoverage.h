#pragma once

#include <memory>
#include <vector>
#include "S100_TemporalExtent.h"
#include "EX_BoundingPolygon.h"

namespace S100 {
    class S100_DataCoverage
    {
    public:
        EX_BoundingPolygon BoundingPolygon;
        std::shared_ptr<S100_TemporalExtent> TemporalExtent;
        std::shared_ptr<int> OptimumDisplayScale;
        std::shared_ptr<int> MaximumDisplayScale;
        std::shared_ptr<int> MinimumDisplayScale;
        std::vector<double> ApproximateGridResolution;

        void GetContents(pugi::xml_node& node);
    };
}
