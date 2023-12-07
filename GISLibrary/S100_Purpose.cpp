#include "stdafx.h"
#include "S100_Purpose.h"

namespace S100 {
    std::string S100_PurposeToString(S100_Purpose category) {
        switch (category) {
        case S100_Purpose::newDataset: return "newDataset";
        case S100_Purpose::newEdition: return "newEdition";
        case S100_Purpose::update: return "update";
        case S100_Purpose::reissue: return "reissue";
        case S100_Purpose::cancellation: return "cancellation";
        case S100_Purpose::delta: return "delta";
        default: throw std::invalid_argument("Unknown category");
        }
    }

    S100_Purpose S100_PurposeFromString(const std::string& categoryName) {
        if (categoryName == "newDataset") return S100_Purpose::newDataset;
        if (categoryName == "newEdition") return S100_Purpose::newEdition;
        if (categoryName == "update") return S100_Purpose::update;
        if (categoryName == "reissue") return S100_Purpose::reissue;
        if (categoryName == "cancellation") return S100_Purpose::cancellation;
        if (categoryName == "delta") return S100_Purpose::delta;
        throw std::invalid_argument("Unknown category name");
    }
}
