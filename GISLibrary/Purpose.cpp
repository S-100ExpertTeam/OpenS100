#include "stdafx.h"
#include "Purpose.h"

namespace S100 {
    std::string S100_PurposeToString(Purpose category) {
        switch (category) {
        case Purpose::newDataset: return "newDataset";
        case Purpose::newEdition: return "newEdition";
        case Purpose::update: return "update";
        case Purpose::reissue: return "reissue";
        case Purpose::cancellation: return "cancellation";
        case Purpose::delta: return "delta";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    Purpose S100_PurposeFromString(const std::string& categoryName) {
        if (categoryName == "newDataset") return Purpose::newDataset;
        if (categoryName == "newEdition") return Purpose::newEdition;
        if (categoryName == "update") return Purpose::update;
        if (categoryName == "reissue") return Purpose::reissue;
        if (categoryName == "cancellation") return Purpose::cancellation;
        if (categoryName == "delta") return Purpose::delta;
        throw std::invalid_argument("Unknown category name");
    }
}
