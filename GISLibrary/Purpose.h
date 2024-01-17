#pragma once

#include <string>
#include <stdexcept>

namespace S100 {
    enum class Purpose {
        newDataset = 1,
        newEdition,
        update,
        reissue,
        cancellation,
        delta
    };

    std::string S100_PurposeToString(Purpose category);
    Purpose S100_PurposeFromString(const std::string& categoryName);
}
