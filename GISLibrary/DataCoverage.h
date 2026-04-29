#pragma once

#include <memory>
#include <vector>
#include "TemporalExtent.h"
#include "EX_BoundingPolygon.h"
#include "EX_GeographicBoundingBox.h"

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

        //temp Bounding Box
        std::shared_ptr<EX_GeographicBoundingBox> BoundingBox;

        void GetContents(pugi::xml_node& node);
        void Save(pugi::xml_node& node);
    };
}
