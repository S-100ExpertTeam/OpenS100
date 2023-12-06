#pragma once

#include <string>
#include <stdexcept>

namespace S100 {
    enum class S100_Purpose {
        newDataset = 1,
        newEdition,
        update,
        reissue,
        cancellation,
        delta
    };

    std::string S100_PurposeToString(S100_Purpose category);
    S100_Purpose S100_PurposeFromString(const std::string& categoryName);
}
